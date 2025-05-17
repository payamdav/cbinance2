#include "frames.hpp"
#include <iostream>
#include "../../trade/tradecache.hpp"


std::ostream& operator<<(std::ostream& os, const Frame& frame) {
    os << "Frame: h=" << frame.h << ", l=" << frame.l << ", vwap=" << frame.vwap << ", n=" << frame.n << ", v=" << frame.v << ", q=" << frame.q;
    return os;
}


Frames::Frames(size_t size, size_t miliseconds) : boost::circular_buffer<Frame>(size) {
    this->miliseconds = miliseconds;
    pubsub.subscribe("slot_" + std::to_string(miliseconds), [this](void* data) { this->new_slot(); });
}

void Frames::new_slot() {
    Frame frame;
    auto index = trade_cache.get_index(miliseconds);
    frame.n = std::distance(*(index.end() - 2), *(index.end() - 1));
    if (frame.n == 0) {
        frame.vwap = this->back().vwap;
        frame.h = this->back().vwap;
        frame.l = this->back().vwap;
    } else {
        frame.h = (*(index.end() - 2))->p;
        frame.l = (*(index.end() - 2))->p;
        for ( auto it = *(index.end() -2); it != *(index.end() - 1); ++it) {
            frame.v += it->v;
            frame.q += it->q;
            if (it->p > frame.h) {
                frame.h = it->p;
            }
            if (it->p < frame.l) {
                frame.l = it->p;
            }
        }
        frame.vwap = frame.q / frame.v;
    }
    this->push_back(frame);
    pubsub.publish("frame_" + std::to_string(miliseconds), &this->back());
}


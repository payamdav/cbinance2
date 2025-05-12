#include "tradecache.hpp"
#include <iostream>


TradeCache::TradeCache(size_t size) {
    this->cache = boost::circular_buffer<Trade>(size);

    this->seconds_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(3600);
    this->five_seconds_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(12 * 60 * 4);
    this->ten_seconds_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(6 * 60 * 8);
    this->thirty_seconds_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(2 * 60 * 24);
    this->minutes_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(60 * 24);
    this->five_minutes_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(12 * 24);
    this->ten_minutes_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(6 * 24);
    this->thirty_minutes_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(2 * 24);
    this->hours_index = boost::circular_buffer<boost::circular_buffer<Trade>::iterator>(24);

    pubsub.subscribe("trade", [this](void* data) { Trade* trade = static_cast<Trade*>(data); this->push(*trade); });
}

TradeCache& TradeCache::getInstance() {
    static TradeCache instance;
    return instance;
}

void TradeCache::push(Trade& trade) {
    this->cache.push_back(trade);
    if (first_trade_ts == 0) {
        first_trade_ts = trade.t;
        seconds_number = ((size_t)(trade.t / 1000)) - 1;
        five_seconds_number = ((size_t)(trade.t / 5000)) - 1;
        ten_seconds_number = ((size_t)(trade.t / 10000)) - 1;
        thirty_seconds_number = ((size_t)(trade.t / 30000)) - 1;
        minutes_number = ((size_t)(trade.t / 60000)) - 1;
        five_minutes_number = ((size_t)(trade.t / 300000)) - 1;
        ten_minutes_number = ((size_t)(trade.t / 600000)) - 1;
        thirty_minutes_number = ((size_t)(trade.t / 1800000)) - 1;
        hours_number = ((size_t)(trade.t / 3600000)) - 1;
    }
    last_trade_ts = trade.t;

    size_t current_seconds_number = trade.t / 1000;
    if (seconds_number != current_seconds_number) {
        for (size_t i = seconds_number + 1; i <= current_seconds_number; i++) {
            seconds_number = i;
            seconds_index.push_back(cache.end() - 1);
            if (seconds_index.size() > 1) pubsub.publish("slot_seconds", nullptr);
        }

        size_t current_five_seconds_number = trade.t / 5000;
        if (five_seconds_number != current_five_seconds_number) {
            for (size_t i = five_seconds_number + 1; i <= current_five_seconds_number; i++) {
                five_seconds_number = i;
                five_seconds_index.push_back(cache.end() - 1);
                if (five_seconds_index.size() > 1) pubsub.publish("slot_five_seconds", nullptr);
            }

            size_t current_ten_seconds_number = trade.t / 10000;
            if (ten_seconds_number != current_ten_seconds_number) {
                for (size_t i = ten_seconds_number + 1; i <= current_ten_seconds_number; i++) {
                    ten_seconds_number = i;
                    ten_seconds_index.push_back(cache.end() - 1);
                    if (ten_seconds_index.size() > 1) pubsub.publish("slot_ten_seconds", nullptr);
                }

                size_t current_thirty_seconds_number = trade.t / 30000;
                if (thirty_seconds_number != current_thirty_seconds_number) {
                    for (size_t i = thirty_seconds_number + 1; i <= current_thirty_seconds_number; i++) {
                        thirty_seconds_number = i;
                        thirty_seconds_index.push_back(cache.end() - 1);
                        if (thirty_seconds_index.size() > 1) pubsub.publish("slot_thirty_seconds", nullptr);
                    }

                    size_t current_minutes_number = trade.t / 60000;
                    if (minutes_number != current_minutes_number) {
                        for (size_t i = minutes_number + 1; i <= current_minutes_number; i++) {
                            minutes_number = i;
                            minutes_index.push_back(cache.end() - 1);
                            if (minutes_index.size() > 1) pubsub.publish("slot_minutes", nullptr);
                        }

                        size_t current_five_minutes_number = trade.t / 300000;
                        if (five_minutes_number != current_five_minutes_number) {
                            for (size_t i = five_minutes_number + 1; i <= current_five_minutes_number; i++) {
                                five_minutes_number = i;
                                five_minutes_index.push_back(cache.end() - 1);
                                if (five_minutes_index.size() > 1) pubsub.publish("slot_five_minutes", nullptr);
                            }

                            size_t current_ten_minutes_number = trade.t / 600000;
                            if (ten_minutes_number != current_ten_minutes_number) {
                                for (size_t i = ten_minutes_number + 1; i <= current_ten_minutes_number; i++) {
                                    ten_minutes_number = i;
                                    ten_minutes_index.push_back(cache.end() - 1);
                                    if (ten_minutes_index.size() > 1) pubsub.publish("slot_ten_minutes", nullptr);
                                }

                                size_t current_thirty_minutes_number = trade.t / 1800000;
                                if (thirty_minutes_number != current_thirty_minutes_number) {
                                    for (size_t i = thirty_minutes_number + 1; i <= current_thirty_minutes_number; i++) {
                                        thirty_minutes_number = i;
                                        thirty_minutes_index.push_back(cache.end() - 1);
                                        if (thirty_minutes_index.size() > 1) pubsub.publish("slot_thirty_minutes", nullptr);
                                    }

                                    size_t current_hours_number = trade.t / 3600000;
                                    if (hours_number != current_hours_number) {
                                        for (size_t i = hours_number + 1; i <= current_hours_number; i++) {
                                            hours_number = i;
                                            hours_index.push_back(cache.end() - 1);
                                            if (hours_index.size() > 1) pubsub.publish("slot_hours", nullptr);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }


}

size_t TradeCache::get_full_duration_in_hours() {
    if (this->cache.size() == 0) return 0;
    return (last_trade_ts - first_trade_ts) / (1000 * 60 * 60);
}

size_t TradeCache::get_memory_used_in_mb() {
    size_t mem_used = 0;
    mem_used += sizeof(this->cache) + this->cache.size() * sizeof(Trade);
    mem_used += sizeof(this->seconds_index) + this->seconds_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    mem_used += sizeof(this->five_seconds_index) + this->five_seconds_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    mem_used += sizeof(this->ten_seconds_index) + this->ten_seconds_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    mem_used += sizeof(this->thirty_seconds_index) + this->thirty_seconds_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    mem_used += sizeof(this->minutes_index) + this->minutes_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    mem_used += sizeof(this->five_minutes_index) + this->five_minutes_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    mem_used += sizeof(this->ten_minutes_index) + this->ten_minutes_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    mem_used += sizeof(this->thirty_minutes_index) + this->thirty_minutes_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    mem_used += sizeof(this->hours_index) + this->hours_index.size() * sizeof(boost::circular_buffer<Trade>::iterator);
    return mem_used / (1024 * 1024);
}


size_t TradeCache::average_count(boost::circular_buffer<boost::circular_buffer<Trade>::iterator> & buf) {
    size_t count = 0;
    for (auto it = buf.begin(); it != buf.end() - 1; ++it) {
        count += std::distance(*it, *(it + 1));
    }
    count += std::distance(buf.back(), cache.end() - 1);
    return count / buf.size();
}

void TradeCache::print_average_counts() {
    std::cout << "Average counts:" << std::endl;
    std::cout << "Seconds: " << average_count(seconds_index) << std::endl;
    std::cout << "Five seconds: " << average_count(five_seconds_index) << std::endl;
    std::cout << "Ten seconds: " << average_count(ten_seconds_index) << std::endl;
    std::cout << "Thirty seconds: " << average_count(thirty_seconds_index) << std::endl;
    std::cout << "Minutes: " << average_count(minutes_index) << std::endl;
    std::cout << "Five minutes: " << average_count(five_minutes_index) << std::endl;
    std::cout << "Ten minutes: " << average_count(ten_minutes_index) << std::endl;
    std::cout << "Thirty minutes: " << average_count(thirty_minutes_index) << std::endl;
    std::cout << "Hours: " << average_count(hours_index) << std::endl;
}


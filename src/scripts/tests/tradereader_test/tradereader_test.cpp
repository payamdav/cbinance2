#include "../../../libs/trade/trade.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include "../../../libs/utils/timer.hpp"
#include "../../../config.hpp"
#include "../../../libs/ta/logscale/logscale.hpp"
#include "../../../libs/core/pubsub/pubsub.hpp"
#include "../../../libs/market/market.hpp"
#include "../../../libs/ta/zigzag/zigzag.hpp"
#include "../../../libs/trade/tradecache.hpp"
#include "../../../libs/ta/frames/frames.hpp"
#include <numeric>


using namespace std;

void test1() {
    string symbol = "btcusdt";
    Logscale logscale(symbol);
    cout << "Logscale for " << symbol << ": " << logscale.base_value << endl;
    TradeReader trade_reader(symbol);
    trade_reader.set_file_cursor(0);
    Trade trade;
    size_t count = trade_reader.count;
    // size_t count = 100000;

    cout << "Number of trades: " << count << endl;

    utils::Timer timer;

    for (size_t i = 0; i < count; ++i) {
        trade_reader.next(trade);
        double l = logscale.scale(trade.p);
        if (i < 10) {
            cout << "Trade " << i << ": "
                 << "Price: " << trade.p
                 << ", Volume: " << trade.v
                 << ", Quantity: " << trade.q
                 << ", Time: " << trade.t
                 << ", Buyer Maker: " << (trade.is_buyer_maker ? "true" : "false")
                 << ", Logscale Price: " << l
                 << endl;
        }
    }
    
    timer.checkpoint("Finished reading trades");
}

class TradeCounter {
    public:
        size_t count;
        TradeCounter() : count(0) {
            PubSub& pubsub = PubSub::getInstance();
            pubsub.subscribe("trade", [this](void* data) { this->new_trade(data); });
            pubsub.subscribe("trade_finished", [this](void* data) { cout << "Trade finished" << endl; });
        }

        void new_trade(void * data) {
            Trade* trade = static_cast<Trade*>(data);
            count++;
        }
};


void test2() {
    string symbol = "btcusdt";
    TradeReader trade_reader(symbol);
    trade_reader.set_file_cursor(0);
    Trade trade;
    TradeCounter trade_counter;

    utils::Timer timer;

    for (size_t i = 0; i < trade_reader.count; ++i) {
        trade_reader.next(trade);
        trade_counter.new_trade(&trade);
    }
    
    timer.checkpoint("Finished reading trades");
    cout << "Total trades counted: " << trade_counter.count << endl;
}


void test3() {
    string symbol = "btcusdt";
    TradeCounter trade_counter;
    TradeReader trade_reader(symbol);
    utils::Timer timer;
    trade_reader.pubsub_trades();
    timer.checkpoint("Finished reading trades");
    cout << "Total trades counted: " << trade_counter.count << endl;
}

void test4() {
    string symbol = "btcusdt";

    Market * market1 = (new Market("Market1"))->set_price_multiplier_to_handle_orders(0.0001)->set_commision(10)->subscribe_to_pubsub();
    Market * market2 = (new Market("Market2"))->set_price_multiplier_to_handle_orders(0.0001)->set_commision(10)->subscribe_to_pubsub();
    Market * market3 = (new Market("Market3"))->set_price_multiplier_to_handle_orders(0.0001)->set_commision(10)->subscribe_to_pubsub();
    Market * market4 = (new Market("Market4"))->set_price_multiplier_to_handle_orders(0.0001)->set_commision(10)->subscribe_to_pubsub();
    Market * market5 = (new Market("Market5"))->set_price_multiplier_to_handle_orders(0.0001)->set_commision(10)->subscribe_to_pubsub();


    TradeReader trade_reader(symbol);
    utils::Timer timer;
    trade_reader.pubsub_trades();
    timer.checkpoint("Finished reading trades");
}


void test5() {
    string symbol = "btcusdt";

    Market * market1 = (new Market("Market1"))->set_price_multiplier_to_handle_orders(0.0001)->set_commision(10)->subscribe_to_pubsub();
    ZigZag * zigzag = (new ZigZag(0.01, 10))->set_publish_appends("zigzag_append")->subscribe_to_pubsub();

    PubSub& pubsub = PubSub::getInstance();
    pubsub.subscribe("zigzag_append", [zigzag](void* data) { 
        cout << "Market1 received zigzag append: " << zigzag->size() << endl;
    });
    
    TradeReader trade_reader(symbol);
    utils::Timer timer;
    trade_reader.pubsub_trades();
    timer.checkpoint("Finished reading trades");
}

void test6() {
    string symbol = "btcusdt";

    Market * market1 = (new Market("Market1"))->set_price_multiplier_to_handle_orders(0.0001)->set_commision(10)->subscribe_to_pubsub();
    TradeCache * trade_cache = &TradeCache::getInstance();

    TradeReader trade_reader(symbol);
    utils::Timer timer;
    trade_reader.pubsub_trades();
    timer.checkpoint("Finished reading trades");

    cout << "TradeCache size: " << trade_cache->cache.size() << endl;
    cout << "TradeCache capacity: " << trade_cache->cache.capacity() << endl;
    cout << "TradeCache first trade: " << trade_cache->cache.front() << endl;
    cout << "TradeCache last trade: " << trade_cache->cache.back() << endl;
    cout << "TradeCache duration: " << (trade_cache->cache.back().t - trade_cache->cache.front().t) / (1000 * 60 * 60) << endl;
    cout << "TradeCache full duration in hours: " << trade_cache->get_full_duration_in_hours() << endl;
    cout << "TradeCache Memory Used: " << (trade_cache->cache.size() * sizeof(Trade)) / (1024 * 1024) << " MB" << endl;
    cout << "TradeCache Memory Used: " << trade_cache->get_memory_used_in_mb() << " MB" << endl;
    cout << "-------------------------" << endl;
    trade_cache->print_average_counts();
}


void test7() {
    string symbol = "btcusdt";

    Market * market1 = (new Market("Market1"))->set_price_multiplier_to_handle_orders(0.0001)->set_commision(10)->subscribe_to_pubsub();
    TradeCache2 * trade_cache = TradeCache2::getInstanceP();
    // trade_cache->add_index(1000, 3600)->add_index(5000, 12 * 60 * 4)->add_index(10000, 6 * 60 * 8)->add_index(30000, 2 * 60 * 24)->add_index(60000, 60 * 24)->add_index(300000, 12 * 24)->add_index(600000, 6 * 24)->add_index(1800000, 2 * 24)->add_index(3600000, 24);
    trade_cache->add_index(1000, 3600)->add_index(60000, 60 * 24);

    TradeReader trade_reader(symbol);
    utils::Timer timer;
    trade_reader.pubsub_trades();
    timer.checkpoint("Finished reading trades");

    cout << "TradeCache size: " << trade_cache->cache.size() << endl;
    cout << "TradeCache capacity: " << trade_cache->cache.capacity() << endl;
    cout << "TradeCache first trade: " << trade_cache->cache.front() << endl;
    cout << "TradeCache last trade: " << trade_cache->cache.back() << endl;
    cout << "TradeCache duration: " << (trade_cache->cache.back().t - trade_cache->cache.front().t) / (1000 * 60 * 60) << endl;
    cout << "TradeCache full duration in hours: " << trade_cache->get_full_duration_in_hours() << endl;
    cout << "TradeCache Memory Used: " << (trade_cache->cache.size() * sizeof(Trade)) / (1024 * 1024) << " MB" << endl;
    cout << "TradeCache Memory Used: " << trade_cache->get_memory_used_in_mb() << " MB" << endl;
    cout << "-------------------------" << endl;
    trade_cache->print_average_counts();
}

void test8() {
    string symbol = "btcusdt";

    TradeCache& trade_cache = TradeCache::getInstance();
    Frames fh(1000, 3600000);
    Frames fm(60000, 60000);
    Frames fs(60000000, 1000);

    TradeReader trade_reader(symbol);
    utils::Timer timer;
    trade_reader.pubsub_trades();
    timer.checkpoint("Finished reading trades");

    cout << "FH size: " << fh.size() << endl;
    cout << "FH capacity: " << fh.capacity() << endl;
    cout << "FH first frame: " << fh.front() << endl;
    cout << "FH last frame: " << fh.back() << endl;
    cout << "FH avg vwap: " << std::accumulate(fh.begin(), fh.end(), 0.0, [](double sum, const Frame& frame) { return sum + frame.vwap; }) / fh.size() << endl;
    cout << "FH Max H: " << std::max_element(fh.begin(), fh.end(), [](const Frame& a, const Frame& b) { return a.h < b.h; })->h << endl;
    cout << "FH Min L: " << std::min_element(fh.begin(), fh.end(), [](const Frame& a, const Frame& b) { return a.l < b.l; })->l << endl;
    cout << "--------------------------" << endl;
    cout << "FM size: " << fm.size() << endl;
    cout << "FM capacity: " << fm.capacity() << endl;
    cout << "FM first frame: " << fm.front() << endl;
    cout << "FM last frame: " << fm.back() << endl;
    cout << "FM avg vwap: " << std::accumulate(fm.begin(), fm.end(), 0.0, [](double sum, const Frame& frame) { return sum + frame.vwap; }) / fm.size() << endl;
    cout << "FM Max H: " << std::max_element(fm.begin(), fm.end(), [](const Frame& a, const Frame& b) { return a.h < b.h; })->h << endl;
    cout << "FM Min L: " << std::min_element(fm.begin(), fm.end(), [](const Frame& a, const Frame& b) { return a.l < b.l; })->l << endl;
    cout << "--------------------------" << endl;
    cout << "FS size: " << fs.size() << endl;
    cout << "FS capacity: " << fs.capacity() << endl;
    cout << "FS first frame: " << fs.front() << endl;
    cout << "FS last frame: " << fs.back() << endl;
    cout << "FS avg vwap: " << std::accumulate(fs.begin(), fs.end(), 0.0, [](double sum, const Frame& frame) { return sum + frame.vwap; }) / fs.size() << endl;
    cout << "FS Max H: " << std::max_element(fs.begin(), fs.end(), [](const Frame& a, const Frame& b) { return a.h < b.h; })->h << endl;
    cout << "FS Min L: " << std::min_element(fs.begin(), fs.end(), [](const Frame& a, const Frame& b) { return a.l < b.l; })->l << endl;
    cout << "FS Count n==0 : " << std::count_if(fs.begin(), fs.end(), [](const Frame& frame) { return frame.n == 0; }) << endl;
    

 }

int main() {
    test8();
    cout << "----------------------------------------" << endl;
    return 0;
}



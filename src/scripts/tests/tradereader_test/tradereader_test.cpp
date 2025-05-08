#include "../../../libs/trade/trade.hpp"
#include <string>
#include <iostream>
#include "../../../libs/utils/timer.hpp"
#include "../../../config.hpp"
#include "../../../libs/ta/logscale/logscale.hpp"

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


int main() {
    test1();
    return 0;
}


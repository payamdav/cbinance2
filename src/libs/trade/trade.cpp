#include "trade.hpp"
#include <fstream>
#include "../../config.hpp"
#include "../utils/file_utils.hpp"

using namespace std;

// operator overload for << to print Trade objects
ostream& operator<<(ostream& os, const Trade& trade) {
    os << "Trade(p: " << trade.p 
       << ", v: " << trade.v 
       << ", q: " << trade.q 
       << ", t: " << trade.t 
       << ", is_buyer_maker: " << (trade.is_buyer_maker ? "true" : "false") 
       << ")";
    return os;
}


bool TradeReader::read_trade(size_t index, Trade &trade) {
    if (index >= count) return false;
    trade_data.seekg(index * sizeof(Trade), ios::beg); // Move to the correct position in the file
    trade_data.read(reinterpret_cast<char*>(&trade), sizeof(Trade)); // Read the trade data
    return true; // Return true if the trade was successfully read
}

Trade TradeReader::read_trade(size_t index) {
    Trade trade;
    if (!read_trade(index, trade)) {
        cout << "Error: Unable to read trade at index: " << index << endl;
        // Handle the error as needed, e.g., return a default trade or throw an exception
    }
    return trade; // Return the trade object
}

Trade TradeReader::read_first() {
    if (count == 0) {
        throw runtime_error("No trades available to read.");
    }
    return read_trade(0); // Read the first trade
}

Trade TradeReader::read_last() {
    if (count == 0) {
        throw runtime_error("No trades available to read.");
    }
    return read_trade(count - 1); // Read the last trade
}

size_t TradeReader::search(size_t t) {
    // Binary search for the trade with timestamp t or greater than t
    size_t left = 0;
    size_t right = count;
    size_t mid;
    Trade mid_trade;
    while (left < right) {
        mid = left + (right - left) / 2; // Calculate the middle index
        read_trade(mid, mid_trade); // Read the trade at the middle index
        if (mid_trade.t < t) {
            left = mid + 1; // Search in the right half
        } else {
            right = mid; // Search in the left half
        }
    }
    return left; // Return the index of the first trade with timestamp >= t    
}


TradeReader::TradeReader(string symbol) {
    this->symbol = symbol;
    size_t file_size = utils::get_file_size(DATA_PATH + "um/trades/" + this->symbol + ".bin");
    this->count = file_size / sizeof(Trade); // Calculate the number of trades in the binary file
    open(); // Open the binary file for reading
}

TradeReader::~TradeReader() {
    close(); // Close the file if it's open
}

void TradeReader::open() {
    trade_data.open(DATA_PATH + "um/trades/" + this->symbol + ".bin", ios::in | ios::binary); // Open the binary file for reading
    if (!trade_data.is_open()) {
        cout << "Error: Could not open trade data file: " << symbol << endl;
        return; // Return early if the file cannot be opened
    }
}

void TradeReader::close() {
    if (trade_data.is_open()) {
        trade_data.close(); // Close the file if it's open
    } else {
        cout << "Trade data file for symbol: " << symbol << " is not open." << endl;
    }
}


void TradeReader::set_file_cursor(size_t pos) {
    trade_data.seekg(pos * sizeof(Trade), ios::beg); // Move to the specified position
}

void TradeReader::next(Trade &trade) {
    trade_data.read(reinterpret_cast<char*>(&trade), sizeof(Trade)); // Read the next trade
}


#pragma once

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

using namespace std;


inline string HOME_PATH = "/home/payam/";
inline string DATA_PATH = HOME_PATH + "data/";
inline string FILES_PATH = DATA_PATH + "files/";
inline array<string, 7> SYMBOLS = {"adausdt", "btcusdt", "ethusdt", "vineusdt", "trumpusdt", "xrpusdt", "dogeusdt"};
inline unordered_map<string, double> SYMBOLS_MIN_PRICE = {
    {"adausdt", 0.6124},
    {"btcusdt", 76560},
    {"ethusdt", 1747.47},
    {"vineusdt", 0.02323},
    {"trumpusdt", 8.729},
    {"xrpusdt", 1.8952},
    {"dogeusdt", 0.14269}
};




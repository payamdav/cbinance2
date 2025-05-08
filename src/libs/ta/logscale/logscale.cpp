#include "logscale.hpp"
#include <cmath>
#include "../../../config.hpp"
#include <iostream>
#include <string>

using namespace std;

Logscale::Logscale(double base_value, double multiplier) {
    this->base_value = base_value;
    this->multiplier = multiplier;
    this->log_base = 1.0 + multiplier;
    this->log_base_log2 = log2(log_base);
    this->base_value_log2 = log2(base_value);
}

Logscale::Logscale(string symbol) {
    base_value = SYMBOLS_MIN_PRICE[symbol];
    multiplier = 0.0001;
    log_base = 1.0 + multiplier;
    log_base_log2 = log2(log_base);
    base_value_log2 = log2(base_value);
}

double Logscale::scale(const double & value) {
    return (log2(value) - base_value_log2) / log_base_log2;
}

long Logscale::scale_int(const double & value) {
    return (long) ((log2(value) - base_value_log2) / log_base_log2);
}

double Logscale::operator () (const double & value) {
    return scale(value);
}


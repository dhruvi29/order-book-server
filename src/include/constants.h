#include <cinttypes>
#include <iostream>
#include <vector>
#include <string>
#include <map>

enum class OrderType {
    LIMIT,
    MARKET,
};

enum class OrderDirection {
    BUY,
    SELL
};

using Price = float;
using Quantity = uint32_t;
using OrderId = uint64_t;

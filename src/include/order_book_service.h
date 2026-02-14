#include "./order_book.h"

class OrderBookService {

    static OrderBook book;
    public:
    static OrderBook GetOrderBook();
    static void AddOrder(Price, Quantity, OrderDirection);

};
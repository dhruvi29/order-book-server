#include "./include/order_book_service.h"

OrderBook OrderBookService::book = OrderBook();

OrderBook OrderBookService::GetOrderBook(){
    return book;
}

void OrderBookService::AddOrder(Price price, Quantity quantity, OrderDirection direction){
    book.AddOrder(price, quantity, direction);
}


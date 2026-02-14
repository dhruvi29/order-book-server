#include "./include/order.h"

Order::Order (OrderId orderId, Price price, Quantity quantity, OrderDirection orderDirection, OrderType orderType) : 
    id{orderId},
    price{price}, 
    initialQuantity{quantity},
    remainingQuantity{quantity},
    direction{orderDirection} ,
    orderType{orderType}
    {}

Price Order::GetPrice() { return price;}
Quantity Order::GetInitialQuantity() { return initialQuantity;}
Quantity Order::GetRemainingQuantity() { return remainingQuantity;}
Quantity Order::GetFilledQuantity() { return GetInitialQuantity() - GetFilledQuantity(); }
OrderDirection Order::GetOrderDirection() { return direction;}
OrderType Order::GetOrderType() { return orderType;}
OrderId Order::GetOrderId() { return id;}

void Order::Fill( Quantity fillQuantity) {
    if (fillQuantity > GetRemainingQuantity()) {
        throw std::logic_error("Cannot fill the order " + std::to_string(GetOrderId()) + ". Fill quantity is more than remaining quantity.");
    }

    remainingQuantity -= fillQuantity;
}

void Order::Display() {
    std::cout << "Id: " << id << " ";
    std::cout << "Price: " << price << " ";
    std::cout << "Initial Quantity: " << initialQuantity << " ";
    std::cout << "Remaining Quantity: " << remainingQuantity << " ";
    std::cout << std::endl;
}

LimitOrder::LimitOrder (OrderId orderId, Price price, Quantity quantity, OrderDirection orderDirection) 
: Order(orderId ,price, quantity, orderDirection,OrderType::LIMIT) 
{}

MarketOrder::MarketOrder (OrderId orderId, Price price, Quantity quantity, OrderDirection orderDirection) 
: Order(orderId, price, quantity, orderDirection,OrderType::MARKET) 
{}
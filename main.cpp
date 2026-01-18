#include <iostream>
#include <numbers>
#include <cinttypes>
#include <map>
#include <vector>

using Price = float;
using Quantity = uint32_t;
using OrderId = uint64_t;


enum class OrderType {
    LIMIT_ORDER,
    MARKET_ORDER,
};

enum class OrderDirection {
    BUY,
    SELL
};

class Order {

    const Price price;
    const Quantity initialQuantity;
    Quantity remainingQuantity;
    const OrderDirection direction;
    const OrderType orderType;
    const OrderId id;

public:
    Order (OrderId orderId, Price price, Quantity quantity, OrderDirection orderDirection, OrderType orderType) : 
    id{orderId},
    price{price}, 
    initialQuantity{quantity},
    remainingQuantity{quantity},
    direction{orderDirection} ,
    orderType{orderType}
    {} 

    Price GetPrice() { return price;}
    Quantity GetInitialQuantity() { return initialQuantity;}
    Quantity GetRemainingQuantity() { return remainingQuantity;}
    Quantity GetFilledQuantity() { return GetInitialQuantity() - GetFilledQuantity(); }
    OrderDirection GetOrderDirection() { return direction;}
    OrderType GetOrderType() { return orderType;}
    OrderId GetOrderId() { return id;}

    void Fill( Quantity fillQuantity) {
        if (fillQuantity > GetRemainingQuantity()) {
            throw std::logic_error("Cannot fill the order " + std::to_string(GetOrderId()) + ". Fill quantity is more than remaining quantity.");
        }

        remainingQuantity -= fillQuantity;
    }

    void Display() {
        std::cout << "Id: " << id << " ";
        std::cout << "Price: " << price << " ";
        std::cout << "Initial Quantity: " << initialQuantity << " ";
        std::cout << "Remaining Quantity: " << remainingQuantity << " ";
        std::cout << std::endl;
    }

};

struct LimitOrder : Order {
    public:
    LimitOrder (OrderId orderId, Price price, Quantity quantity, OrderDirection orderDirection) 
    : Order(orderId ,price, quantity, orderDirection,OrderType::LIMIT_ORDER) 
    {}
};

// Not using this for now
struct MarketOrder : Order {
    public:
    MarketOrder (OrderId orderId, Price price, Quantity quantity, OrderDirection orderDirection) 
    : Order(orderId, price, quantity, orderDirection,OrderType::MARKET_ORDER) 
    {}
};

using OrderList = std::vector<Order>;
using BuyListType = std::map<Price, OrderList, std::greater<Price>>;
using SellListType = std::map<Price, OrderList>;

class OrderBook {

    // Thought process on selecting the data structure for maintaining list of Orders 
    // std::vector<Order> sellList;
    // 1. Vector - But i need the list to be sorted after insertion and deletion
    // 2. Priority Queue - But I should be able to randomly access the element to cancel or modify it
    // 3. Trying Maps - It was mostly good, however will fail when there are multiple orders for same price
    // std::map<Price, Order, std::greater<Price>> buyList;
    // std::map<Price, Order> sellList;
    BuyListType buyList;
    SellListType sellList;
    
    public:
    
    static OrderId ordersCount;
    OrderBook() {    }

    void AddOrder(Price price, Quantity quantity, OrderDirection dir) {
        LimitOrder newOrder = LimitOrder(ordersCount, price, quantity, dir);
        ordersCount++;
        if (dir == OrderDirection::BUY) {
            if (buyList.find(price) == buyList.end()) buyList[price] = OrderList();
            buyList[price].push_back(newOrder);
        } else {
            if (sellList.find(price) == sellList.end()) sellList[price] = OrderList();
            sellList[price].push_back(newOrder);
        }
        Match();
    }
    void AddBuyOrder(Price price, Quantity quantity) {
        LimitOrder newOrder = LimitOrder(ordersCount, price, quantity, OrderDirection::BUY);
        ordersCount++;
        if (buyList.find(price) == buyList.end()) buyList[price] = OrderList();
        buyList[price].push_back(newOrder);

        newOrder.Display();
        Match();
    }
    void AddSellOrder(Price price, Quantity quantity) {
        LimitOrder newOrder = LimitOrder(ordersCount, price, quantity, OrderDirection::SELL);
        ordersCount++;
        if (sellList.find(price) == sellList.end()) sellList[price] = OrderList();
        sellList[price].push_back(newOrder);

        newOrder.Display();
        Match();
    }

    Price GetBestBid() {

        if (IsBidsListEmpty()) {
            throw std::logic_error("Buy list is empty");
        }
        return buyList.begin() -> first;
    }
    
    Price GetBestAsk() {
        if (IsAsksListEmpty()) {
            throw std::logic_error("Sell list is empty");
        }
        return sellList.begin() -> first;
    }

    Price GetMidPrice() {
        return ( GetBestAsk() + GetBestBid() ) / 2;
    }

    Price GetMarketDepth() {

        if (sellList.empty() || buyList.empty()) {
            throw std::logic_error("Either Buy or Sell list is empty");
        }

        Price buyMin = buyList.rbegin() -> first;
        Price sellMax = sellList.rbegin() -> first;

        return sellMax - buyMin;
    }

    void CleanBuys() {
        // Cleaning best buys
        Price bestPrice = buyList.begin() -> first;
        OrderList oList = buyList.begin() -> second;
        OrderList newList = OrderList();

        for (int i = 0; i < oList.size(); i++) {
            Order &item = oList[i];
            if (item.GetRemainingQuantity() > 0) newList.push_back(item);
            // else: add order to tradebook because it is completed TODO
        }
        if (newList.empty()) buyList.erase(bestPrice);
        else buyList[bestPrice] = OrderList(newList);

    }

    void CleanSells() {
        // Cleaning best sells
        Price bestPrice = sellList.begin() -> first;
        OrderList oList = sellList.begin() -> second;
        OrderList newList = OrderList();
        for (int i = 0 ; i < oList.size(); i++) {
            Order &item = oList[i];
            if (item.GetRemainingQuantity() > 0) newList.push_back(item);
        }
        if (newList.empty()) sellList.erase(bestPrice);
        else sellList[bestPrice] = OrderList(newList);
    }

    bool IsBidsListEmpty() {
        return buyList.empty();
    }

    bool IsAsksListEmpty() {
        return sellList.empty();
    }

    void Match() {
        while (!IsBidsListEmpty() && !IsAsksListEmpty()) {
            Price bestBid = GetBestBid();
            Price bestAsk = GetBestAsk();
            if ( bestBid < bestAsk ) break; // No matching possible
            else {
                OrderList &bestBids = buyList.begin() -> second;
                OrderList &bestAsks = sellList.begin() -> second;

                int bidIdx = 0, askIdx = 0;
                while ( bidIdx < bestBids.size() && askIdx < bestAsks.size() ) {

                    Order &bid = bestBids[bidIdx];
                    Order &ask = bestAsks[askIdx];
                    Quantity matchQty = std::min(bid.GetRemainingQuantity(), ask.GetRemainingQuantity());
                    
                    bid.Fill(matchQty);
                    ask.Fill(matchQty);

                    Price matchPrice = getMatchPrice_(bid, ask);

                    // Make Trade
                    std::cout << "Order Executed @" << matchPrice << " for " << matchQty << " shares" << std::endl;

                    if (bid.GetRemainingQuantity() == 0) bidIdx++;
                    if (ask.GetRemainingQuantity() == 0) askIdx++;

                }
                
                CleanBuys();
                CleanSells();
            }

        }
    }

    Price getMatchPrice_(Order &bid, Order &ask) {
        
        if (bid.GetOrderId() < ask.GetOrderId()) return bid.GetPrice();
        else return ask.GetPrice();

    }


    
};


OrderId OrderBook::ordersCount = 0;

int main ( ) {

    OrderBook orderbook = OrderBook();

    orderbook.AddBuyOrder(250, 3);
    std::cout << orderbook.GetBestBid() << "\n";
    orderbook.AddBuyOrder(260, 5);
    std::cout << orderbook.GetBestBid() << "\n";
    orderbook.AddBuyOrder(245, 3);
    std::cout << orderbook.GetBestBid() << "\n";
    orderbook.AddSellOrder(265, 3);
    std::cout << orderbook.GetBestAsk() << "\n";
    orderbook.AddSellOrder(266, 3);
    std::cout << orderbook.GetBestAsk() << "\n";
    orderbook.AddSellOrder(258, 3);
    orderbook.AddSellOrder(260, 3);
    orderbook.AddBuyOrder(266, 5);
    std::cout << orderbook.GetBestAsk() << "\n";

    return 0;
}


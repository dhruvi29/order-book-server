#include "./order.h"

using OrderList = std::vector<Order>;
using BuyListType = std::map<Price, std::vector<Order>, std::greater<Price>>;
using SellListType = std::map<Price, std::vector<Order>>;


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
    OrderBook(){};
    void AddOrder(Price price, Quantity quantity, OrderDirection dir);        
    void AddBuyOrder(Price price, Quantity quantity);
    void AddSellOrder(Price price, Quantity quantity);
    Price GetBestBid();
    Price GetBestAsk();
    Price GetMidPrice();
    Price GetMarketDepth();
    void CleanBuys();
    void CleanSells();
    bool IsBidsListEmpty();
    bool IsAsksListEmpty();
    void Match();
    Price getMatchPrice_(Order &bid, Order &ask);


    
};

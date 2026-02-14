#include "./include/order_book.h"

OrderId OrderBook::ordersCount = 0;

void OrderBook::AddOrder(Price price, Quantity quantity, OrderDirection dir) {
    LimitOrder newOrder = LimitOrder(OrderBook::ordersCount, price, quantity, dir);
    OrderBook::ordersCount++;
    if (dir == OrderDirection::BUY) {
        if (buyList.find(price) == buyList.end()) buyList[price] = OrderList();
        buyList[price].push_back(newOrder);
    } else {
        if (sellList.find(price) == sellList.end()) sellList[price] = OrderList();
        sellList[price].push_back(newOrder);
    }
    
    newOrder.Display();
    Match();
}
void OrderBook::AddBuyOrder(Price price, Quantity quantity) {
    LimitOrder newOrder = LimitOrder(OrderBook::ordersCount, price, quantity, OrderDirection::BUY);
    OrderBook::ordersCount++;
    if (buyList.find(price) == buyList.end()) buyList[price] = OrderList();
    buyList[price].push_back(newOrder);

    newOrder.Display();
    Match();
}
void OrderBook::AddSellOrder(Price price, Quantity quantity) {
    LimitOrder newOrder = LimitOrder(OrderBook::ordersCount, price, quantity, OrderDirection::SELL);
    OrderBook::ordersCount++;
    if (sellList.find(price) == sellList.end()) sellList[price] = OrderList();
    sellList[price].push_back(newOrder);

    newOrder.Display();
    Match();
}

Price OrderBook::GetBestBid() {

    if (IsBidsListEmpty()) {
        throw std::logic_error("Buy list is empty");
    }
    return buyList.begin() -> first;
}

Price OrderBook::GetBestAsk() {
    if (IsAsksListEmpty()) {
        throw std::logic_error("Sell list is empty");
    }
    return sellList.begin() -> first;
}

Price OrderBook::GetMidPrice() {
    return ( GetBestAsk() + GetBestBid() ) / 2;
}

Price OrderBook::GetMarketDepth() {

    if (sellList.empty() || buyList.empty()) {
        throw std::logic_error("Either Buy or Sell list is empty");
    }

    Price buyMin = buyList.rbegin() -> first;
    Price sellMax = sellList.rbegin() -> first;

    return sellMax - buyMin;
}

void OrderBook::CleanBuys() {
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

void OrderBook::CleanSells() {
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

bool OrderBook::IsBidsListEmpty() {
    return buyList.empty();
}

bool OrderBook::IsAsksListEmpty() {
    return sellList.empty();
}

void OrderBook::Match() {
    std::cout << "Matching Algo begins\n";
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

Price OrderBook::getMatchPrice_(Order &bid, Order &ask) {
    
    if (bid.GetOrderId() < ask.GetOrderId()) return bid.GetPrice();
    else return ask.GetPrice();

}
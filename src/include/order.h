#include "./constants.h"

class Order {

    const Price price;
    const Quantity initialQuantity;
    Quantity remainingQuantity;
    const OrderDirection direction;
    const OrderType orderType;
    const OrderId id;

public:
    Order (OrderId, Price, Quantity, OrderDirection, OrderType);

    Price GetPrice();
    Quantity GetInitialQuantity();
    Quantity GetRemainingQuantity();
    Quantity GetFilledQuantity();
    OrderDirection GetOrderDirection();
    OrderType GetOrderType();
    OrderId GetOrderId();

    void Fill( Quantity );

    void Display();

};

struct LimitOrder : Order {
    public:
    LimitOrder (OrderId, Price, Quantity, OrderDirection);
};

// Not using this for now
struct MarketOrder : Order {
    public:
    MarketOrder (OrderId, Price, Quantity, OrderDirection);
};

#include "PriceBroadcaster.h"

void PriceBroadcaster::start()
{
    drogon::app().getLoop()->runEvery(
        1.0, // every 1 second
        [] {
            static double price = 100.0;

            // simulate price movement
            price += ((rand() % 100) - 50) * 0.01;

            std::string msg = std::to_string(price);

            for (auto &c : WsMarketController::clients) {
                if (c && c->connected())
                    c->send(msg);
            }
        }
    );
}
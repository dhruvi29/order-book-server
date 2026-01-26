#include "WsMarketController.h"

std::set<WebSocketConnectionPtr> WsMarketController::clients;

void WsMarketController::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
    // write your application logic here
    std::cout << "Message received: " << message << std::endl;
    for (auto &c : WsMarketController::clients) {
        c->send(message);
    }
}

void WsMarketController::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
    std::cout << "New connection received" << std::endl;
    WsMarketController::clients.insert(wsConnPtr);

}

void WsMarketController::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
    std::cout << "Connection closed" << std::endl;
    WsMarketController::clients.erase(wsConnPtr);

}

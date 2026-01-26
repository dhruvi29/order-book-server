#pragma once

#include <drogon/WebSocketController.h>
#include <set>

using namespace drogon;

class WsMarketController : public drogon::WebSocketController<WsMarketController>
{
  public:
    static std::set<WebSocketConnectionPtr> clients;

     void handleNewMessage(const WebSocketConnectionPtr&,
                                  std::string &&,
                                  const WebSocketMessageType &) override;
    void handleNewConnection(const HttpRequestPtr &,
                                     const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;
    WS_PATH_LIST_BEGIN
    // list path definitions here;
    // WS_PATH_ADD("/path", "filter1", "filter2", ...);
    WS_PATH_ADD("/price");
    WS_PATH_LIST_END
};

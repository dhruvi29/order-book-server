#include <iostream>
#include <json/json.h>
#include "OrderController.h"
#include "../src/include/order_book_service.h"

void OrderController::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    // write your application logic here
    std::cout << "Recieved Order \n";
    std::cout << req->bodyData() << "\n";

    // Parse the JSON body
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errs;
    std::string body = req->bodyData();
    std::istringstream s(body);
    if (!Json::parseFromStream(builder, s, &root, &errs)) {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        // Optionally, return an error response here
        return;
    }

    // Extract fields
    std::string direction = root.get("direction", "").asString();
    Quantity quantity = std::stoi(root.get("quantity", "0").asString());
    Price price = std::stod(root.get("price", "0").asString());

    // Map direction string to enum
    OrderDirection dir = (direction == "buy") ? OrderDirection::BUY : OrderDirection::SELL;

    // Now you can use dir, quantity, price as needed
    OrderBookService::AddOrder(price, quantity, dir);
    std::cout << "Order placed\n";

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("Hello, World!");
    callback(resp); // Send response back

    
}

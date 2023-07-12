#include "crow_all.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/route")
    .methods("POST"_method)
    ([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        std::cout << "xd" << std::endl;
    });

    app.port(4000).multithreaded().run();
}
#include "crow_all.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/route")
    .methods("POST"_method)
    ([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);
        std::vector<std::string> addresses = x["addresses"];
        // Aquí es donde llamarías a tu algoritmo TSP
        // Por ejemplo:
        // TSP tsp(addresses);
        // std::vector<std::string> shortest_route = tsp.get_shortest_route();
        crow::json::wvalue result;
        result["route"] = shortest_route; // Asume que shortest_route es un vector de strings
        return crow::response(result);
    });

    app.port(4000).multithreaded().run();
}
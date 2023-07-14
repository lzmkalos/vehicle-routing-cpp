#include "crow_all.h"
#include "cpr/cpr.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::string API_KEY = "AIzaSyBYEGtbRbSiHCjSkdR0yRtOVE_iyOAu9tQ";

// API para obtener la matriz de distancias

void get_and_save_distances(const std::vector<std::string>& addresses){
    // Obtener la matriz de distancias
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<int>> time_matrix;

    for (int i = 0; i < addresses.size(); i++) {
        std::vector<int> row;
        std::vector<int> time_row;

        for (int j = 0; j < addresses.size(); j++) {
            if (i == j) {
                row.push_back(0);
                time_row.push_back(0);
            } else {
                auto response = cpr::Get(cpr::Url{"https://maps.googleapis.com/maps/api/directions/json"},
                                        cpr::Parameters{
                                            {"origin", addresses[i]},
                                            {"destination", addresses[j]},
                                            {"key", API_KEY}
                                        });
                auto result = json::parse(response.text);
                if (result["status"] == "OK") {
                    int distance = result["routes"][0]["legs"][0]["distance"]["value"];
                    int duration = result["routes"][0]["legs"][0]["duration"]["value"];
                    row.push_back(distance);
                    time_row.push_back(duration);
                } else {
                    row.push_back(INT_MAX);
                    time_row.push_back(INT_MAX);
                }
            }
        }
        matrix.push_back(row);
        time_matrix.push_back(time_row);
    };

    // guardar la matriz de distancias en un archivo
    std::ofstream file("distance_matrix.txt");
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            file << elem << ' ';
        }
        file << '\n';
    }
    file.close();

    // guardar la matriz de tiempos en un archivo
    std::ofstream time_file("time_matrix.txt");
    for (const auto& row : time_matrix) {
        for (const auto& elem : row) {
            time_file << elem << ' ';
        }
        time_file << '\n';
    }
    time_file.close();
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/autocomplete/<string>")
    ([](const std::string& input) {
        auto response = cpr::Get(cpr::Url{"https://maps.googleapis.com/maps/api/place/autocomplete/json"},
                                  cpr::Parameters{{"input", input}, {"key", "AIzaSyBYEGtbRbSiHCjSkdR0yRtOVE_iyOAu9tQ"}});
        crow::response res(response.text);
        res.set_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    CROW_ROUTE(app, "/submit")
    .methods("OPTIONS"_method)
    ([](const crow::request& req) {
        crow::response res;
        res.code = 200;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res;
    });

    CROW_ROUTE(app, "/submit")
    .methods("POST"_method)
    ([](const crow::request& req) {
        crow::response res;
        auto json = crow::json::load(req.body);
        if (!json) {
            res.code = 400;
        } else {
            std::vector<std::string> locations;
            for (const auto& location : json["locations"]) {
                locations.push_back(location.s());
            }
            // Llama a la función get_and_save_distances
            get_and_save_distances(locations);
            res.code = 200;
            res.set_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Headers", "Content-Type");
        }
        return res;
    });

    app.port(4040).multithreaded().run();
};
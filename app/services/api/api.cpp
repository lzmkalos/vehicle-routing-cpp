#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string API_KEY = "AIzaSyBYEGtbRbSiHCjSkdR0yRtOVE_iyOAu9tQ";

std::vector<std::string> addresses = {
    "Oregon+Foods+Planta",
    "Av.+Javier+Prado+Este+2010,+Lima+15036",  // La rambla
    "C.+Los+Libertadores+302,+San+Isidro+Lima+15073", // Long Horn San Isidro
    "Av.+Elmer+Faucett,+Lima+07001",  // Makro Callao
    "Jr.+Ucello+162,+San+Borja+Lima+15036"  // Wong San Borja
};

std::vector<std::vector<int>> get_distances() {
    std::vector<std::vector<int>> matrix;
    for (int i = 0; i < addresses.size(); i++) {
        std::vector<int> row;
        for (int j = 0; j < addresses.size(); j++) {
            if (i == j) {
                row.push_back(0);
            } else {
                auto response = cpr::Get(cpr::Url{"https://maps.googleapis.com/maps/api/distancematrix/json"},
                                          cpr::Parameters{
                                              {"origins", addresses[i]},
                                              {"destinations", addresses[j]},
                                              {"key", API_KEY}
                                          });
                auto result = json::parse(response.text);
                if (result["rows"][0]["elements"][0]["status"] == "OK") {
                    row.push_back(result["rows"][0]["elements"][0]["distance"]["value"]);
                } else {
                    row.push_back(INT_MAX);
                }
            }
        }
        matrix.push_back(row);
    }
    return matrix;
}

void save_to_file(const std::vector<std::vector<int>>& matrix) {
    std::ofstream file("./data/db/distance_matrix.txt");
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            file << elem << ' ';
        }
        file << '\n';
    }
}

int main() {
    auto distance_matrix = get_distances();
    save_to_file(distance_matrix);
    return 0;
}
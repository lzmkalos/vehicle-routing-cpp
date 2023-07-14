#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>

// ############################
// Clase Grafo
// ############################

class Graph {
private:
    std::vector<std::vector<int>> lista_adyacencia; //Matriz de adyacencia
    std::vector<std::vector<int>> lista_tiempos;    //Matriz de tiempos
public:
    // Constructor
    Graph(int numero_nodos): lista_adyacencia(numero_nodos, std::vector<int>(numero_nodos, 1)),
                            lista_tiempos(numero_nodos, std::vector<int>(numero_nodos, 1)) {};

    // Getters
    int get_peso(int nodo1, int nodo2) const {
        return lista_adyacencia[nodo1][nodo2];
    };

    // Metodos
    void agregar_arista(int nodo1, int nodo2, int peso) {
        if (nodo1 < lista_adyacencia.size() && nodo2 < lista_adyacencia[nodo1].size()) {
            lista_adyacencia[nodo1][nodo2] = peso;
        } else {
            std::cout << "Error: los índices están fuera de los límites de la matriz de adyacencia." << std::endl;
        }
    };

    void imprimir_grafo() {
        // Tabla de matriz de adyacencia
        std::cout << "Tabla de matriz de adyacencia:" << std::endl;
        for (const auto &fila : lista_adyacencia) {
            for (int peso : fila) {
                std::cout << peso << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    };

    void readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        int i = 0;
        while (std::getline(file, line)) {
            // Ignorar las líneas en blanco
            if (line.empty()) continue;

            std::istringstream iss(line);
            int j = 0;
            int weight;
            while (iss >> weight) {
                agregar_arista(i, j, weight);
                ++j;
            }
            ++i;
        }
        file.close();

        //Leemos la matriz de tiempos desde el otro archivo
        std::ifstream timeFile("./data/db/time_matrix.txt");
        i = 0;
        while (std::getline(timeFile, line)) {
            // Ignorar las líneas en blanco
            if (line.empty()) continue;

            std::istringstream iss(line);
            int j = 0;
            int time;
            while (iss >> time) {
                lista_tiempos[i][j] = time;
                ++j;
            }
            ++i;
        }
        timeFile.close();
    }

    // diagrama con graphviz
    void generar_dot(const std::string& filename) const {
        std::ofstream file(filename);
        file << "digraph {\n";
        for (int i = 0; i < lista_adyacencia.size(); ++i) {
            for (int j = 0; j < lista_adyacencia[i].size(); ++j) {
                if (lista_adyacencia[i][j] != 0) {
                    file << i << " -> " << j << " [label=\"" << lista_adyacencia[i][j] << "\"];\n";
                }
            }
        }
        file << "}\n";
        file.close();
    }


    // TSP
// TSP
    void tsp() {
        std::vector<int> vertice_inicial;
        for (int i = 0; i < lista_adyacencia.size(); i++)
            if (i != 0)
                vertice_inicial.push_back(i);

        int camino_mas_corto = std::numeric_limits<int>::max();
        std::vector<int> ruta_mas_corta;

        int camino_mas_rapido = std::numeric_limits<int>::max();
        std::vector<int> ruta_mas_rapida;

        do {
            int camino_peso_actual = 0;
            int camino_peso_rapido_actual = 0;

            int k = 0;
            for (int i = 0; i < vertice_inicial.size(); i++) {
                camino_peso_actual += lista_adyacencia[k][vertice_inicial[i]];
                camino_peso_rapido_actual += lista_tiempos[k][vertice_inicial[i]];
                k = vertice_inicial[i];
            }
            camino_peso_actual += lista_adyacencia[k][0];
            camino_peso_rapido_actual += lista_tiempos[k][0];

            if (camino_peso_actual < camino_mas_corto) {
                camino_mas_corto = camino_peso_actual;
                ruta_mas_corta = vertice_inicial;
            }

            if (camino_peso_rapido_actual < camino_mas_rapido) {
                camino_mas_rapido = camino_peso_rapido_actual;
                ruta_mas_rapida = vertice_inicial;
            }

    } while (std::next_permutation(vertice_inicial.begin(), vertice_inicial.end()));

    std::cout << "Camino mas corto con distancia: " << camino_mas_corto << '\n';
    std::cout << "El camino es: 0 ";
    for (int i : ruta_mas_corta) {
        std::cout << "-> " << i;
    }
    std::cout << " -> 0\n";

    std::cout << "Camino mas rapido con tiempo: " << camino_mas_rapido << '\n';
    std::cout << "El camino es: 0 ";
    for (int i : ruta_mas_rapida) {
        std::cout << "-> " << i;
    }
    std::cout << " -> 0\n";
}
};


// ############################
// Main
// ############################

int main() {

    Graph g(5);
    g.readFromFile("./data/db/distance_matrix.txt");
    g.imprimir_grafo();
    g.tsp();
    g.generar_dot("./data/helpers/graph.dot");

    system("dot -Tpng ./data/helpers/graph.dot -o ./data/images/graph.png");
    return 0;
}
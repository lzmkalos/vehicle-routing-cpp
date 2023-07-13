#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

typedef adjacency_list<listS, vecS, directedS, no_property, property<edge_weight_t, int>> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

class GraphND {
private:
    Graph graph;

public:
    GraphND() {}

    void addEdge(int u, int v, int weight) {
        add_edge(u, v, weight, graph);
    }

    void createGraph(const std::vector<std::vector<int>>& distances) {
        int numPlaces = distances.size();
        for (int i = 0; i < numPlaces; ++i) {
            for (int j = 0; j < numPlaces; ++j) {
                addEdge(i, j, distances[i][j]);
            }
        }
    }

    std::vector<int> calculateShortestPath(Vertex start, Vertex target) const {
        std::vector<Vertex> predecessors(num_vertices(graph));
        std::vector<int> distances(num_vertices(graph));

        dijkstra_shortest_paths(graph, start,
            predecessor_map(&predecessors[0]).distance_map(&distances[0]));

        std::vector<int> path;
        Vertex current = target;
        while (current != start) {
            path.push_back(current);
            current = predecessors[current];
        }
        path.push_back(start);

        std::reverse(path.begin(), path.end());
        return path;
    }

    int calculateDistance(const std::vector<int>& path) const {
        int distance = 0;
        for (int i = 0; i < path.size() - 1; ++i) {
            Vertex u = path[i];
            Vertex v = path[i + 1];
            distance += get(edge_weight, graph, edge(u, v, graph).first);
        }
        return distance;
    }

    int numVertices() const {
        return num_vertices(graph);
    }
};

class RoutePlanner {
private:
    GraphND graph;
    int numCars;
    std::vector<bool> visited;
    std::vector<int> currentRoute;
    std::vector<int> bestRoute;
    int minDistance;

public:
    RoutePlanner(const std::vector<std::vector<int>>& distances, int numCars)
        : numCars(numCars), visited(distances.size(), false),
          currentRoute(distances.size() * numCars), bestRoute(distances.size() * numCars),
          minDistance(std::numeric_limits<int>::max()) {
        graph.createGraph(distances);
    }

    void dfs(int currentPlace, int currentDistance, int depth) {
        visited[currentPlace] = true;
        currentRoute[depth] = currentPlace;

        if (depth == graph.numVertices() - 1) {
            int distance = currentDistance + graph.calculateDistance(currentRoute);
            if (distance < minDistance) {
                minDistance = distance;
                bestRoute = currentRoute;
            }
            visited[currentPlace] = false;
            return;
        }

        for (int nextPlace = 0; nextPlace < graph.numVertices(); ++nextPlace) {
            if (!visited[nextPlace]) {
                dfs(nextPlace, currentDistance + graph.calculateDistance({currentPlace, nextPlace}), depth + 1);
            }
        }

        visited[currentPlace] = false;
    }

    void findBestRoute() {
        dfs(0, 0, 0);
    }

    void printBestRoute() const {
        int numPlaces = graph.numVertices();
        std::cout << "Mejor ruta:\n";
        for (int i = 0; i < numCars; ++i) {
            std::cout << "Carro " << i << ":\n";
            for (int j = 0; j < numPlaces; ++j) {
                int index = i * numPlaces + j;
                std::cout << bestRoute[index] << " -> ";
            }
            std::cout << bestRoute[i * numPlaces] << "\n";
        }
        std::cout << "Distancia mínima: " << minDistance << " unidades de distancia\n";
    }
};  

int main() {
    std::vector<std::vector<int>> distances =
    {
        {0, 39098, 38682, 37847, 55855, 41944, 35948, 34686},
        {38143, 0, 8188, 3674, 20286, 11451, 9945, 14081},
        {35961, 7126, 0, 5567, 23883, 4803, 3327, 7433},
        {36845, 3119, 6142, 0, 19478, 9404, 8647, 12035},
        {55734, 20482, 14215, 20562, 0, 10320, 25034, 17336},
        {39879, 11043, 3921, 9485, 10425, 0, 6967, 4392},
        {32650, 8970, 4050, 7720, 25728, 8249, 0, 5486},
        {33115, 13757, 6634, 12198, 16116, 4981, 5421, 0}
    };

    int numCars = 1;

    RoutePlanner planner(distances, numCars);
    planner.findBestRoute();
    planner.printBestRoute();

    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <limits>

#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_traits.hpp>

int main() {
    const int num_vertices = 10;

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    typedef boost::adjacency_matrix<boost::undirectedS, boost::no_property,
                                    boost::property<boost::edge_weight_t, int>> Graph;
    Graph g(num_vertices);

    int matrix[num_vertices][num_vertices];
    for (int i = 0; i < num_vertices; ++i) {
        matrix[i][i] = 0;
        for (int j = i + 1; j < num_vertices; ++j) {
            int weight = dist(engine);
            boost::add_edge(i, j, weight, g);
            matrix[i][j] = weight;
            matrix[j][i] = weight;
        }
    }

    std::cout << "Graph Adjacency Matrix (edge weights):" << std::endl;
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::vector<int> vertices(num_vertices - 1);
    std::iota(vertices.begin(), vertices.end(), 1);

    int min_cost = std::numeric_limits<int>::max();
    std::vector<int> best_path;

    auto weight_map = boost::get(boost::edge_weight, g);

    do {
        int current_cost = 0;

        auto e_first = boost::edge(0, vertices[0], g);
        current_cost += weight_map[e_first.first];

        for (size_t i = 0; i < vertices.size() - 1; ++i) {
            auto e = boost::edge(vertices[i], vertices[i+1], g);
            current_cost += weight_map[e.first];
        }

        auto e_last = boost::edge(vertices.back(), 0, g);
        current_cost += weight_map[e_last.first];

        if (current_cost < min_cost) {
            min_cost = current_cost;
            best_path.clear();
            best_path.push_back(0);
            best_path.insert(best_path.end(), vertices.begin(), vertices.end());
            best_path.push_back(0);
        }

    } while (std::next_permutation(vertices.begin(), vertices.end()));

    std::cout << "Optimal vertex traversal sequence:" << std::endl;
    for (size_t i = 0; i < best_path.size(); ++i) {
        std::cout << best_path[i] << (i == best_path.size() - 1 ? "" : " -> ");
    }
    std::cout << std::endl;
    std::cout << "Minimum total cost: " << min_cost << " USD" << std::endl;
}

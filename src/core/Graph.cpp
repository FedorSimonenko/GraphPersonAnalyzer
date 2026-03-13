#include "core/Graph.hpp"
#include "utils/Exceptions.hpp"
#include <iostream>

namespace graph_analyzer {

Graph::Graph(const std::unordered_map<short, std::unordered_set<short>>& graph) 
    : adjacencyList(graph) {}

bool Graph::hasVertex(short vertex) const {
    return adjacencyList.find(vertex) != adjacencyList.end();
}

bool Graph::hasEdge(short from, short to) const {
    if (!hasVertex(from)) return false;
    return adjacencyList.at(from).find(to) != adjacencyList.at(from).end();
}

std::unordered_set<short>& Graph::operator[](short vertex) {
    return adjacencyList[vertex];
}

const std::unordered_set<short>& Graph::operator[](short vertex) const {
    auto it = adjacencyList.find(vertex);
    if (it != adjacencyList.end()) {
        return it->second;
    }
    throw GraphException("Vertex not found: " + std::to_string(vertex));
}

std::vector<std::pair<short, short>> Graph::getAllEdges() const {
    std::vector<std::pair<short, short>> edges;
    for (const auto& [from, neighbors] : adjacencyList) {
        for (short to : neighbors) {
            if (from < to) {
                edges.emplace_back(from, to);
            }
        }
    }
    return edges;
}

void Graph::print() const {
    for (const auto& [vertex, neighbors] : adjacencyList) {
        std::cout << vertex << ": ";
        for (short n : neighbors) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }
}

}

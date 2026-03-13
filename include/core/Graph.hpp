#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

namespace graph_analyzer {

class Graph {
private:
    std::unordered_map<short, std::unordered_set<short>> adjacencyList;

public:
    Graph() = default;
    explicit Graph(const std::unordered_map<short, std::unordered_set<short>>& graph);
    
    bool hasVertex(short vertex) const;
    bool hasEdge(short from, short to) const;
    
    std::unordered_set<short>& operator[](short vertex);
    const std::unordered_set<short>& operator[](short vertex) const;
    
    std::vector<std::pair<short, short>> getAllEdges() const;
    void print() const;
    
    static constexpr size_t getMaxVertices() { return 1000; }
};

}

#endif

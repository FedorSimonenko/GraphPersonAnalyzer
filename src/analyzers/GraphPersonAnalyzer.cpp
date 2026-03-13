#include "analyzers/GraphPersonAnalyzer.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>

namespace graph_analyzer {

    GraphPersonAnalyzer::GraphPersonAnalyzer(const Graph& g)
            : graph(g), normalizer(std::make_unique<DataNormalizer>()) {}

    void GraphPersonAnalyzer::addPerson(std::shared_ptr<const Person> person) {
        for (const auto& route : person->routes) {
            short from = route.first;
            short to = route.second;

            if (graph.hasEdge(from, to)) {
                if (from > to) std::swap(from, to);
                edgePeople[{from, to}].push_back(person);
            }
        }
    }

    void GraphPersonAnalyzer::addPeople(const std::vector<std::shared_ptr<const Person>>& people) {
        for (const auto& person : people) {
            addPerson(person);
        }
    }

    void GraphPersonAnalyzer::calculateAverages() {
        edgeAverages.clear();
        for (const auto& [edge, people] : edgePeople) {
            edgeAverages[edge] = EdgeAverage(people);
        }
    }

    void GraphPersonAnalyzer::normalize() {
        std::vector<EdgeAverage> averages;
        for (const auto& [_, avg] : edgeAverages) {
            averages.push_back(avg);
        }

        std::vector<Person> allPeople;
        for (const auto& [_, people] : edgePeople) {
            for (const auto& p : people) {
                allPeople.push_back(*p);
            }
        }

        normalizer->fit(allPeople);
        normalizer->fitFromAverages(averages);
        isNormalized = true;
    }

    double GraphPersonAnalyzer::euclideanDistance(const std::vector<double>& v1,
                                                  const std::vector<double>& v2) const {
        if (v1.size() != v2.size()) return std::numeric_limits<double>::max();

        double sum = 0.0;
        for (size_t i = 0; i < v1.size(); ++i) {
            double diff = v1[i] - v2[i];
            sum += diff * diff;
        }
        return std::sqrt(sum);
    }

    std::optional<double> GraphPersonAnalyzer::analyze(const Person& person) const {
        if (!isNormalized) return std::nullopt;

        auto [closestEdge, distance] = findClosestEdgeNormalized(person);
        if (closestEdge.has_value()) {
            return distance;
        }
        return std::nullopt;
    }

    std::vector<double> GraphPersonAnalyzer::getNormalizedPerson(const Person& person) const {
        if (!isNormalized) {
            throw std::runtime_error("Data not normalized yet. Call normalize() first.");
        }
        return normalizer->normalizePerson(person);
    }

    std::vector<double> GraphPersonAnalyzer::getNormalizedEdge(short from, short to) const {
        if (!isNormalized) {
            throw std::runtime_error("Data not normalized yet. Call normalize() first.");
        }

        if (from > to) std::swap(from, to);
        auto it = edgeAverages.find({from, to});
        if (it != edgeAverages.end()) {
            return normalizer->normalizeEdgeAverage(it->second);
        }
        return {};
    }

    double GraphPersonAnalyzer::normalizedEuclideanDistance(const Person& person,
                                                            short from, short to) const {
        auto personVec = getNormalizedPerson(person);
        auto edgeVec = getNormalizedEdge(from, to);

        if (edgeVec.empty()) return std::numeric_limits<double>::max();
        return euclideanDistance(personVec, edgeVec);
    }

    std::pair<std::optional<std::pair<short, short>>, double>
    GraphPersonAnalyzer::findClosestEdgeNormalized(const Person& person) const {
        if (edgeAverages.empty() || !isNormalized) {
            return {std::nullopt, 0.0};
        }

        auto personVec = getNormalizedPerson(person);
        std::optional<std::pair<short, short>> bestEdge;
        double minDistance = std::numeric_limits<double>::max();

        for (const auto& [edge, avg] : edgeAverages) {
            auto edgeVec = normalizer->normalizeEdgeAverage(avg);
            double dist = euclideanDistance(personVec, edgeVec);

            if (dist < minDistance) {
                minDistance = dist;
                bestEdge = edge;
            }
        }

        return {bestEdge, minDistance};
    }

    std::vector<std::pair<std::pair<short, short>, double>>
    GraphPersonAnalyzer::getAllNormalizedDistances(const Person& person) const {
        std::vector<std::pair<std::pair<short, short>, double>> distances;

        if (!isNormalized) return distances;

        auto personVec = getNormalizedPerson(person);

        for (const auto& [edge, avg] : edgeAverages) {
            auto edgeVec = normalizer->normalizeEdgeAverage(avg);
            double dist = euclideanDistance(personVec, edgeVec);
            distances.emplace_back(edge, dist);
        }

        std::sort(distances.begin(), distances.end(),
                  [](const auto& a, const auto& b) { return a.second < b.second; });

        return distances;
    }

    std::optional<EdgeAverage> GraphPersonAnalyzer::getEdgeAverage(short from, short to) const {
        if (from > to) std::swap(from, to);
        auto it = edgeAverages.find({from, to});
        if (it != edgeAverages.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void GraphPersonAnalyzer::printStatistics() const {
        std::cout << "\n=== Edge statistics ===" << std::endl;
        for (const auto& [edge, avg] : edgeAverages) {
            std::cout << "Edge (" << edge.first << "," << edge.second << "):" << std::endl;
            std::cout << "  ";
            avg.print();
            std::cout << "\n Quantity of persons: " << edgePeople.at(edge).size() << std::endl;
        }

        if (isNormalized) {
            normalizer->printParams();
        }
    }

    const DataNormalizer& GraphPersonAnalyzer::getNormalizer() const {
        return *normalizer;
    }

}
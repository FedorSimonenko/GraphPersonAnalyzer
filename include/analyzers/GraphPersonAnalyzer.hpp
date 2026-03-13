#ifndef GRAPH_PERSON_ANALYZER_HPP
#define GRAPH_PERSON_ANALYZER_HPP

#include "core/Graph.hpp"
#include "core/Person.hpp"
#include "analyzers/EdgeAverage.hpp"
#include "analyzers/DataNormalizer.hpp"
#include "core/IAnalyzer.hpp"
#include <unordered_map>
#include <vector>
#include <optional>
#include <memory>
#include <algorithm>
#include <cmath>
#include <limits>

namespace graph_analyzer {

    struct PairHash {
        template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };

    class GraphPersonAnalyzer : public IAnalyzer {
    private:
        const Graph& graph;
        std::unordered_map<std::pair<short, short>, std::vector<std::shared_ptr<const Person>>, PairHash> edgePeople;
        std::unordered_map<std::pair<short, short>, EdgeAverage, PairHash> edgeAverages;

        std::unique_ptr<DataNormalizer> normalizer;
        bool isNormalized = false;

        double euclideanDistance(const std::vector<double>& v1, const std::vector<double>& v2) const;

    public:
        explicit GraphPersonAnalyzer(const Graph& g);

        GraphPersonAnalyzer(const GraphPersonAnalyzer&) = delete;
        GraphPersonAnalyzer& operator=(const GraphPersonAnalyzer&) = delete;

        GraphPersonAnalyzer(GraphPersonAnalyzer&&) = default;
        GraphPersonAnalyzer& operator=(GraphPersonAnalyzer&&) = default;

        ~GraphPersonAnalyzer() = default;

        void addPerson(std::shared_ptr<const Person> person);
        void addPeople(const std::vector<std::shared_ptr<const Person>>& people);

        void calculateAverages();
        void normalize();

        std::optional<double> analyze(const Person& person) const override;
        std::string getName() const override {return "GraphPersonAnalyzer";}
        bool isInitialized() const override {return isNormalized;}

        std::vector<double> getNormalizedPerson(const Person& person) const;
        std::vector<double> getNormalizedEdge(short from, short to) const;

        double normalizedEuclideanDistance(const Person& person, short from, short to) const;

        std::pair<std::optional<std::pair<short, short>>, double>findClosestEdgeNormalized(const Person& person) const;
        std::vector<std::pair<std::pair<short, short>, double>>getAllNormalizedDistances(const Person& person) const;

        std::optional<EdgeAverage> getEdgeAverage(short from, short to) const;
        void printStatistics() const;
        const DataNormalizer& getNormalizer() const;
    };

}

#endif
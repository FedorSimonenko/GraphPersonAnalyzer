#include <iostream>
#include <memory>
#include <vector>
#include <variant>
#include "core/Graph.hpp"
#include "core/Person.hpp"
#include "filters/AgeFilter.hpp"
#include "filters/GenderFilter.hpp"
#include "filters/CompositeFilter.hpp"
#include "analyzers/GraphPersonAnalyzer.hpp"
#include "io/CSVReader.hpp"

using namespace graph_analyzer;

int main() {
    try {
        std::cout << "=== Graph Person Analyzer ===\n" << std::endl;

        std::unordered_map<short, std::unordered_set<short>> initialGraph = {
                {1, {2, 3}},
                {2, {1, 4}},
                {3, {1, 5}},
                {4, {2}},
                {5, {3}}
        };

        Graph graph(initialGraph);
        std::cout << "The graph has been created:" << std::endl;
        graph.print();

        std::vector<std::shared_ptr<const Person>> people;
        people.push_back(std::make_shared<Person>("Ivan", "Ivanov", 25, true, true, 70, 175,
                                                  std::vector<std::pair<short, short>>{{1, 2}, {2, 4}}));
        people.push_back(std::make_shared<Person>("Petr", "Petrov", 30, true, false, 80, 180,
                                                  std::vector<std::pair<short, short>>{{1, 2}, {1, 3}}));
        people.push_back(std::make_shared<Person>("Maria", "Sidorovna", 28, false, true, 60, 165,
                                                  std::vector<std::pair<short, short>>{{1, 3}, {3, 5}}));
        people.push_back(std::make_shared<Person>("Anna", "Kozlova", 35, false, true, 65, 170,
                                                  std::vector<std::pair<short, short>>{{2, 4}, {1, 2}}));

        std::cout << "\n=== Testing filters ===" << std::endl;
        auto ageFilter = std::make_unique<AgeFilter>(25, 35);
        auto genderFilter = std::make_unique<GenderFilter>(true); // мужчины

        std::cout << "Filter 1: " << ageFilter->getDescription() << std::endl;
        std::cout << "Filter 2: " << genderFilter->getDescription() << std::endl;

        auto combinedFilter = (*ageFilter) && (*genderFilter);
        std::cout << "Combined filter: " << combinedFilter->getDescription() << std::endl;

        std::cout << "\nFiltering results:" << std::endl;
        for (const auto& person : people) {
            bool passesAge = ageFilter->passes(*person);
            bool passesGender = genderFilter->passes(*person);
            bool passesCombined = combinedFilter->passes(*person);

            std::cout << person->name << " " << person->surname << ": "
                      << "Age=" << passesAge
                      << ", Gender=" << passesGender
                      << ", Combined=" << passesCombined << std::endl;
        }

        std::cout << "\n=== Graph analysis ===" << std::endl;
        GraphPersonAnalyzer analyzer(graph);
        analyzer.addPeople(people);
        analyzer.calculateAverages();
        analyzer.normalize();

        analyzer.printStatistics();

        Person newPerson{"NewName", "NewSurname", 32, true, true, 75, 178, {}};

        std::cout << "\n=== New Person Analysis ===" << std::endl;
        auto [closestEdge, distance] = analyzer.findClosestEdgeNormalized(newPerson);

        if (closestEdge.has_value()) {
            std::cout << "Nearest edge: (" << closestEdge->first
                      << "," << closestEdge->second << ")" << std::endl;
            std::cout << "Normalized distance: " << distance << std::endl;
            std::cout << "\nAll distances:" << std::endl;
            auto allDistances = analyzer.getAllNormalizedDistances(newPerson);
            for (const auto& [edge, dist] : allDistances) {
                std::cout << "  (" << edge.first << "," << edge.second << "): "
                          << dist << std::endl;
            }
        }
        std::cout << "\n=== CSVReader Testing ===" << std::endl;
        CSVReader csvReader("test.csv");

        auto readResult = csvReader.readAll("test.csv");
        if (readResult.has_value()) {
            std::cout << "Read " << readResult->size() << " records" << std::endl;
        } else {
            std::cout << "The file was not found, this is normal for the test" << std::endl;
        }

        std::cout << "\n=== Variant Testing ===" << std::endl;
        std::variant<int, double, std::string> result;
        result = 42;
        std::cout << "Result (int): " << std::get<int>(result) << std::endl;
        result = 3.14;
        std::cout << "Result (double): " << std::get<double>(result) << std::endl;

        auto visitor = [](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
            std::cout << "The user found int: " << arg << std::endl;
        } else if constexpr (std::is_same_v<T, double>) {
            std::cout << "The user found double: " << arg << std::endl;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "The user found string: " << arg << std::endl;
        }
        };

        result = std::string("hello");
        std::visit(visitor, result);

    } catch (const GraphAnalyzerException& e) {
        std::cerr << "Analyzer error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
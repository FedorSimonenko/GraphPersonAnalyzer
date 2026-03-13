#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <queue>
#include <unordered_set>
#include "core/Graph.hpp"
#include "core/Person.hpp"
#include "filters/AgeFilter.hpp"
#include "filters/GenderFilter.hpp"
#include "filters/CompositeFilter.hpp"
#include "analyzers/GraphPersonAnalyzer.hpp"
#include "io/CSVReader.hpp"

using namespace graph_analyzer;
using namespace testing;

namespace fs = std::filesystem;

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        graph = std::make_unique<Graph>(createMetroGraph());
        createTestPeople();
        testFilename = "test_routes.csv";
        if (fs::exists(testFilename)) fs::remove(testFilename);
    }
    void TearDown() override {
        if (fs::exists(testFilename)) fs::remove(testFilename);
    }
    std::unordered_map<short, std::unordered_set<short>> createMetroGraph() {
        return {
                {1, {2, 5}}, {2, {1, 3, 6}}, {3, {2, 4, 7}},
                {4, {3, 8}}, {5, {1, 6, 9}}, {6, {2, 5, 7, 10}},
                {7, {3, 6, 8, 11}}, {8, {4, 7, 12}}, {9, {5, 10}},
                {10, {6, 9, 11}}, {11, {7, 10, 12}}, {12, {8, 11}}
        };
    }
    void createTestPeople() {
        students = {
                std::make_shared<Person>("Anna", "Smith", 19, false, true, 55, 165,
                                         std::vector<std::pair<short, short>>{{1, 2}, {2, 6}, {6, 10}}),
                std::make_shared<Person>("Peter", "Jones", 20, true, true, 65, 172,
                                         std::vector<std::pair<short, short>>{{5, 6}, {6, 7}, {7, 11}}),
                std::make_shared<Person>("Maria", "Garcia", 21, false, false, 58, 168,
                                         std::vector<std::pair<short, short>>{{9, 10}, {10, 11}, {11, 12}})
        };

        workers = {
                std::make_shared<Person>("John", "Williams", 32, true, true, 78, 178,
                                         std::vector<std::pair<short, short>>{{1, 5}, {5, 9}, {9, 10}}),
                std::make_shared<Person>("Olivia", "Brown", 29, false, true, 62, 170,
                                         std::vector<std::pair<short, short>>{{2, 3}, {3, 7}, {7, 11}}),
                std::make_shared<Person>("David", "Miller", 35, true, false, 85, 182,
                                         std::vector<std::pair<short, short>>{{4, 8}, {8, 12}, {12, 11}})
        };

        athletes = {
                std::make_shared<Person>("Alex", "Johnson", 24, true, true, 90, 195,
                                         std::vector<std::pair<short, short>>{{6, 10}, {10, 11}, {11, 7}}),
                std::make_shared<Person>("Helen", "Davis", 26, false, true, 72, 185,
                                         std::vector<std::pair<short, short>>{{3, 7}, {7, 8}, {8, 4}}),
                std::make_shared<Person>("Sergey", "Wilson", 27, true, false, 88, 192,
                                         std::vector<std::pair<short, short>>{{5, 6}, {6, 2}, {2, 1}})
        };

        seniors = {
                std::make_shared<Person>("Nick", "Anderson", 68, true, true, 70, 172,
                                         std::vector<std::pair<short, short>>{{1, 2}, {2, 3}, {3, 4}}),
                std::make_shared<Person>("Tatyana", "Martinez", 65, false, true, 63, 164,
                                         std::vector<std::pair<short, short>>{{8, 7}, {7, 6}, {6, 5}})
        };

        allPeople.insert(allPeople.end(), students.begin(), students.end());
        allPeople.insert(allPeople.end(), workers.begin(), workers.end());
        allPeople.insert(allPeople.end(), athletes.begin(), athletes.end());
        allPeople.insert(allPeople.end(), seniors.begin(), seniors.end());
    }

    std::vector<std::pair<short, short>> pathToRoutes(const std::vector<short>& path) {
        std::vector<std::pair<short, short>> routes;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            routes.emplace_back(path[i], path[i + 1]);
        }
        return routes;
    }

    std::unique_ptr<Graph> graph;
    std::vector<std::shared_ptr<const Person>> students, workers, athletes, seniors, allPeople;
    std::string testFilename;
};

// TEST 1: Filter functionality test
TEST_F(IntegrationTest, FilterTest) {
    auto ageFilter = std::make_unique<AgeFilter>(18, 25);
    auto genderFilter = std::make_unique<GenderFilter>(false);
    auto combined = (*ageFilter) && (*genderFilter);

    int count = 0;
    for (const auto& p : allPeople) {
        if (combined->passes(*p)) count++;
    }

    EXPECT_EQ(count, 2);  // Anna Smith, Maria Garcia (18-25 years, female)
}

// TEST 2: CSV read/write test
TEST_F(IntegrationTest, CSVTest) {
    CSVReader reader(testFilename);

    EXPECT_TRUE(reader.writePerson(*students[0], testFilename));
    EXPECT_TRUE(reader.writePerson(*workers[0], testFilename));

    auto result = reader.readAll(testFilename);
    ASSERT_TRUE(result.has_value());

    auto readPeople = result.value();
    ASSERT_EQ(readPeople.size(), 2);
    EXPECT_EQ(readPeople[0].name, "Anna");
    EXPECT_EQ(readPeople[0].surname, "Smith");
    EXPECT_EQ(readPeople[1].name, "John");
    EXPECT_EQ(readPeople[1].surname, "Williams");
}

// TEST 3: Analyzer functionality test
TEST_F(IntegrationTest, AnalyzerTest) {
    GraphPersonAnalyzer analyzer(*graph);
    analyzer.addPeople(allPeople);
    analyzer.calculateAverages();
    analyzer.normalize();

    Person newPerson("Test", "User", 20, true, true, 68, 175, {});

    auto [closestEdge, distance] = analyzer.findClosestEdgeNormalized(newPerson);

    ASSERT_TRUE(closestEdge.has_value());
    auto avg = analyzer.getEdgeAverage(closestEdge->first, closestEdge->second);
    ASSERT_TRUE(avg.has_value());

    EXPECT_GT(distance, 0);
    EXPECT_LT(distance, 2);
}

// TEST 4: COMPLETE CYCLE - building route for a person
TEST_F(IntegrationTest, FullRouteBuildingTest) {
    GraphPersonAnalyzer analyzer(*graph);
    analyzer.addPeople(allPeople);
    analyzer.calculateAverages();
    analyzer.normalize();

    Person tourist("James", "Bond", 28, true, false, 75, 180, {});

    std::cout << "\n=== Building route for tourist ===\n";
    std::cout << "Tourist: " << tourist.name << " " << tourist.surname
              << ", age: " << tourist.age << ", weight: " << tourist.weight
              << ", height: " << tourist.height << "\n\n";

    auto allDistances = analyzer.getAllNormalizedDistances(tourist);
    ASSERT_GE(allDistances.size(), 3);

    std::cout << "Top-3 closest edges for tourist:\n";
    for (size_t i = 0; i < 3; ++i) {
        auto [edge, dist] = allDistances[i];
        auto avg = analyzer.getEdgeAverage(edge.first, edge.second);
        std::cout << "  " << i+1 << ". Edge (" << edge.first << "," << edge.second
                  << "), distance: " << dist << "\n";
        if (avg) {
            std::cout << "     Average age on this edge: " << avg->avgAge << "\n";
        }
    }

    short currentStation = 1;
    std::vector<short> path = {currentStation};

    for (int step = 0; step < 3; ++step) {
        auto distances = analyzer.getAllNormalizedDistances(tourist);

        short nextStation = -1;
        double bestDist = 1e9;

        for (const auto& [edge, dist] : distances) {
            if (edge.first == currentStation &&
                std::find(path.begin(), path.end(), edge.second) == path.end()) {
                if (dist < bestDist) {
                    bestDist = dist;
                    nextStation = edge.second;
                }
            }
            if (edge.second == currentStation &&
                std::find(path.begin(), path.end(), edge.first) == path.end()) {
                if (dist < bestDist) {
                    bestDist = dist;
                    nextStation = edge.first;
                }
            }
        }

        if (nextStation != -1) {
            path.push_back(nextStation);
            currentStation = nextStation;
        } else {
            break;
        }
    }

    ASSERT_GE(path.size(), 2);

    auto touristRoutes = pathToRoutes(path);
    Person updatedTourist = tourist;
    updatedTourist.routes = touristRoutes;

    std::cout << "\nBuilt route: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i < path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";

    CSVReader reader(testFilename);
    EXPECT_TRUE(reader.writePerson(updatedTourist, testFilename));

    auto result = reader.readAll(testFilename);
    ASSERT_TRUE(result.has_value());

    auto savedPeople = result.value();
    ASSERT_EQ(savedPeople.size(), 1);
    EXPECT_EQ(savedPeople[0].name, "James");
    EXPECT_EQ(savedPeople[0].surname, "Bond");
    EXPECT_EQ(savedPeople[0].routes.size(), touristRoutes.size());

    std::cout << "\n Tourist successfully saved to file " << testFilename << "\n";
    std::cout << "   Route saved: ";
    for (const auto& r : savedPeople[0].routes) {
        std::cout << "(" << r.first << ";" << r.second << ")";
        if (&r != &savedPeople[0].routes.back()) std::cout << "|";
    }
    std::cout << "\n";
}

// TEST 5: Similar people should get similar routes
TEST_F(IntegrationTest, SimilarPeopleSimilarRoutesTest) {
    GraphPersonAnalyzer analyzer(*graph);
    analyzer.addPeople(allPeople);
    analyzer.calculateAverages();
    analyzer.normalize();

    Person p1("Emma", "Watson", 20, false, true, 60, 170, {});
    Person p2("Sophia", "Turner", 21, false, true, 62, 172, {});

    auto routes1 = analyzer.findClosestEdgeNormalized(p1);
    auto routes2 = analyzer.findClosestEdgeNormalized(p2);

    EXPECT_EQ(routes1.first, routes2.first);

    CSVReader reader(testFilename);
    EXPECT_TRUE(reader.writePerson(p1, testFilename));
    EXPECT_TRUE(reader.writePerson(p2, testFilename));

    auto result = reader.readAll(testFilename);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

// TEST 6: Filter and CSV integration test
TEST_F(IntegrationTest, FilterAndCSVTest) {
    CSVReader reader(testFilename);

    for (const auto& s : students) {
        EXPECT_TRUE(reader.writePerson(*s, testFilename));
    }

    auto result = reader.readAll(testFilename);
    ASSERT_TRUE(result.has_value());

    auto saved = result.value();
    ASSERT_EQ(saved.size(), 3);

    AgeFilter ageFilter(18, 22);
    GenderFilter genderFilter(false);
    auto combined = ageFilter && genderFilter;

    int count = 0;
    for (const auto& p : saved) {
        if (combined->passes(p)) count++;
    }

    EXPECT_EQ(count, 2);  // Anna Smith and Maria Garcia (18-22, female)
}
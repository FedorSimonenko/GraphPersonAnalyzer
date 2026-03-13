#include <iostream>
#include <memory>
#include <vector>
#include <variant>
#include <optional>
#include <string>
#include <limits>
#include <iomanip>
#include <fstream>
#include "core/Graph.hpp"
#include "core/Person.hpp"
#include "filters/AgeFilter.hpp"
#include "filters/GenderFilter.hpp"
#include "filters/ResidenceFilter.hpp"
#include "filters/CompositeFilter.hpp"
#include "analyzers/GraphPersonAnalyzer.hpp"
#include "io/CSVReader.hpp"

using namespace graph_analyzer;

void clearScreen() { system("cls"); }
void pressEnterToContinue() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(50, '=') << "\n";
}

class InteractiveApp {
private:
    std::unique_ptr<Graph> graph;
    std::unique_ptr<GraphPersonAnalyzer> analyzer;
    std::vector<std::shared_ptr<const Person>> allPeople;

    void createGraphManually() {
        printHeader("CREATE GRAPH MANUALLY");
        int vertices, edges;
        std::cout << "Enter number of vertices: ";
        std::cin >> vertices;
        std::unordered_map<short, std::unordered_set<short>> graphData;
        for (int i = 1; i <= vertices; i++) {
            graphData[i] = {};
        }

        std::cout << "Enter number of edges: ";
        std::cin >> edges;

        std::cout << "Enter edges (from to):\n";
        for (int i = 0; i < edges; i++) {
            short from, to;
            std::cin >> from >> to;
            graphData[from].insert(to);
            graphData[to].insert(from);
        }

        graph = std::make_unique<Graph>(graphData);
        std::cout << "\nGraph created successfully!\n";
    }

    void loadGraphFromFile() {
        printHeader("LOAD GRAPH FROM FILE");
        std::string filename;
        std::cout << "Enter filename: ";
        std::cin >> filename;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Cannot open file. Using default graph.\n";
            createDefaultGraph();
            return;
        }

        std::unordered_map<short, std::unordered_set<short>> graphData;
        int vertices, edges;
        file >> vertices >> edges;

        for (int i = 1; i <= vertices; i++) {
            graphData[i] = {};
        }
        for (int i = 0; i < edges; i++) {
            short from, to;
            file >> from >> to;
            graphData[from].insert(to);
            graphData[to].insert(from);
        }
        graph = std::make_unique<Graph>(graphData);
        std::cout << "Graph loaded from " << filename << "\n";
    }

    void createDefaultGraph() {
        std::unordered_map<short, std::unordered_set<short>> graphData = {
                {1, {2, 3}},
                {2, {1, 4}},
                {3, {1, 5}},
                {4, {2}},
                {5, {3}}
        };
        graph = std::make_unique<Graph>(graphData);
        std::cout << "Default graph created.\n";
    }

    void addPersonManually() {
        printHeader("ADD PERSON MANUALLY");
        std::string name, surname;
        int age, weight, height, numRoutes;
        bool isMale, isResidense;

        std::cout << "Name: "; std::cin >> name;
        std::cout << "Surname: "; std::cin >> surname;
        std::cout << "Age: "; std::cin >> age;
        std::cout << "Weight (kg): "; std::cin >> weight;
        std::cout << "Height (cm): "; std::cin >> height;
        std::cout << "Gender (1 - male, 0 - female): "; std::cin >> isMale;
        std::cout << "Resident (1 - yes, 0 - no): "; std::cin >> isResidense;
        std::cout << "Number of routes: "; std::cin >> numRoutes;

        std::vector<std::pair<short, short>> routes;
        for (int i = 0; i < numRoutes; i++) {
            short from, to;
            std::cout << "Route " << i+1 << " (from to): ";
            std::cin >> from >> to;
            routes.emplace_back(from, to);
        }

        auto person = std::make_shared<Person>(name, surname, age, isMale, isResidense,
                                               weight, height, routes);
        allPeople.push_back(person);
        if (analyzer) {
            analyzer->addPerson(person);
            analyzer->calculateAverages();
            analyzer->normalize();
        }
        std::cout << "\nPerson added successfully!\n";
    }

    void loadPeopleFromCSV() {
        printHeader("LOAD PEOPLE FROM CSV");
        std::string filename;
        std::cout << "Enter CSV filename: ";
        std::cin >> filename;
        CSVReader reader(filename);
        auto result = reader.readAll(filename);

        if (result.has_value()) {
            auto loaded = result.value();
            for (const auto& p : loaded) {
                auto shared = std::make_shared<Person>(p);
                allPeople.push_back(shared);
                if (analyzer) analyzer->addPerson(shared);
            }
            if (analyzer) {
                analyzer->calculateAverages();
                analyzer->normalize();
            }
            std::cout << "Loaded " << loaded.size() << " people\n";
        } else {
            std::cout << "Could not load file\n";
        }
    }

    void addTestPeople() {
        allPeople = {
                std::make_shared<Person>("Alice", "Walker", 25, false, true, 60, 165,
                                         std::vector<std::pair<short, short>>{{1, 2}, {2, 4}}),

                std::make_shared<Person>("Bob", "Traveler", 30, true, true, 80, 180,
                                         std::vector<std::pair<short, short>>{{1, 3}, {3, 5}}),

                std::make_shared<Person>("Charlie", "Commuter", 35, true, false, 75, 175,
                                         std::vector<std::pair<short, short>>{{1, 2}, {2, 4}}),

                std::make_shared<Person>("Diana", "Tourist", 28, false, true, 65, 170,
                                         std::vector<std::pair<short, short>>{{1, 3}, {3, 5}}),

                std::make_shared<Person>("Eve", "Student", 22, false, false, 58, 168,
                                         std::vector<std::pair<short, short>>{{1, 2}, {2, 4}, {4, 2}}),

                std::make_shared<Person>("Frank", "Athlete", 27, true, true, 85, 190,
                                         std::vector<std::pair<short, short>>{{1, 3}, {3, 5}, {5, 3}}),

                std::make_shared<Person>("Grace", "Senior", 65, false, true, 62, 164,
                                         std::vector<std::pair<short, short>>{{2, 1}, {1, 3}})
        };
        std::cout << "Test data loaded: " << allPeople.size() << " people\n";
    }
    void addMultiplePeople() {
        printHeader("ADD MULTIPLE PEOPLE");
        int count;
        std::cout << "How many people do you want to add? ";
        std::cin >> count;

        for (int i = 0; i < count; i++) {
            std::cout << "\n--- Person " << i+1 << " ---\n";
            addPersonManually();
        }

        if (analyzer) {
            analyzer->calculateAverages();
            analyzer->normalize();
        }
    }

    void setupGraph() {
        printHeader("GRAPH SETUP");
        std::cout << "1. Create graph manually\n";
        std::cout << "2. Load graph from file\n";
        std::cout << "3. Use default graph\n";
        std::cout << "Choose: ";
        int choice;
        std::cin >> choice;

        switch(choice) {
            case 1: createGraphManually(); break;
            case 2: loadGraphFromFile(); break;
            default: createDefaultGraph(); break;
        }
    }

    void setupPeople() {
        printHeader("PEOPLE SETUP");
        std::cout << "1. Add person manually\n";
        std::cout << "2. Load people from CSV\n";
        std::cout << "3. Load test data\n";
        std::cout << "4. Start with empty database\n";
        std::cout << "Choose: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch(choice) {
            case 1: addPersonManually(); break;
            case 2: loadPeopleFromCSV(); break;
            case 3: addTestPeople(); break;
            default: std::cout << "Starting with empty database\n"; break;
        }
    }

    void trainAnalyzer() {
        analyzer = std::make_unique<GraphPersonAnalyzer>(*graph);
        if (!allPeople.empty()) {
            analyzer->addPeople(allPeople);
            analyzer->calculateAverages();
            analyzer->normalize();
        }
    }

    short getMaxVertex() {
        short maxVertex = 0;
        for (short i = 1; i <= 100; i++) {
            if (graph->hasVertex(i)) {
                maxVertex = i;
            }
        }
        return maxVertex;
    }

    int countEdgesWithData() {
        int count = 0;
        auto edges = graph->getAllEdges();
        for (const auto& [from, to] : edges) {
            if (analyzer->getEdgeAverage(from, to).has_value()) {
                count++;
            }
        }
        return count;
    }

public:
    InteractiveApp() {
        clearScreen();
        printHeader("WELCOME TO GRAPH PERSON ANALYZER");

        setupGraph();
        pressEnterToContinue();

        clearScreen();
        setupPeople();

        trainAnalyzer();

        std::cout << "\nInitialization complete!\n";
        pressEnterToContinue();
    }

    void showGraph() {
        printHeader("CURRENT GRAPH");
        if (graph) {
            graph->print();
        } else {
            std::cout << "No graph defined.\n";
        }
    }

    void showAllPeople() {
        printHeader("ALL PEOPLE IN DATABASE");
        if (allPeople.empty()) {
            std::cout << "No people in database.\n";
            return;
        }
        for (size_t i = 0; i < allPeople.size(); ++i) {
            const auto& p = allPeople[i];
            std::cout << i+1 << ". " << p->name << " " << p->surname
                      << ", age: " << p->age
                      << ", weight: " << p->weight
                      << ", height: " << p->height
                      << ", male: " << (p->isMale ? "Yes" : "No")
                      << ", resident: " << (p->isResidense ? "Yes" : "No")
                      << ", routes: " << p->routes.size() << "\n";
        }
    }

    void showEdgeStatistics() {
        printHeader("EDGE STATISTICS");
        if (!analyzer || allPeople.empty()) {
            std::cout << "No data to analyze. Add some people first.\n";
            return;
        }

        auto edges = graph->getAllEdges();

        if (edges.empty()) {
            std::cout << "Graph has no edges.\n";
            return;
        }

        int edgesWithData = countEdgesWithData();
        short maxVertex = getMaxVertex();

        std::cout << "\nGraph Statistics:\n";
        std::cout << "  Total vertices: " << maxVertex << "\n";
        std::cout << "  Total edges: " << edges.size() << "\n";
        if (!edges.empty()) {
            std::cout << "  Edges with people data: " << edgesWithData
                      << " (" << (edgesWithData * 100 / edges.size()) << "%)\n\n";
        }
        analyzer->printStatistics();

        if (edgesWithData == 0) {
            std::cout << "\nNo edge-specific data. Showing overall statistics:\n";
            double avgAge = 0, avgWeight = 0, avgHeight = 0;
            int maleCount = 0, residentCount = 0;

            for (const auto& p : allPeople) {
                avgAge += p->age;
                avgWeight += p->weight;
                avgHeight += p->height;
                if (p->isMale) maleCount++;
                if (p->isResidense) residentCount++;
            }
            avgAge /= allPeople.size();
            avgWeight /= allPeople.size();
            avgHeight /= allPeople.size();

            std::cout << "  Average age: " << std::fixed << std::setprecision(1) << avgAge << "\n";
            std::cout << "  Average weight: " << std::fixed << std::setprecision(1) << avgWeight << " kg\n";
            std::cout << "  Average height: " << std::fixed << std::setprecision(1) << avgHeight << " cm\n";
            std::cout << "  Male/Female: " << maleCount << "/" << (allPeople.size() - maleCount) << "\n";
            std::cout << "  Residents/Non-residents: " << residentCount << "/"
                      << (allPeople.size() - residentCount) << "\n";
        }
    }

    void findClosestEdge() {
        printHeader("FIND CLOSEST EDGE");
        if (!analyzer || allPeople.empty()) {
            std::cout << "No data to analyze. Add some people first.\n";
            return;
        }

        std::string name, surname;
        int age, weight, height;
        bool isMale, isResidense;
        std::cout << "Enter name: "; std::cin >> name;
        std::cout << "Enter surname: "; std::cin >> surname;
        std::cout << "Enter age: "; std::cin >> age;
        std::cout << "Enter weight (kg): "; std::cin >> weight;
        std::cout << "Enter height (cm): "; std::cin >> height;
        std::cout << "Gender (1 - male, 0 - female): "; std::cin >> isMale;
        std::cout << "Resident (1 - yes, 0 - no): "; std::cin >> isResidense;

        Person person(name, surname, age, isMale, isResidense, weight, height, {});

        auto [closestEdge, distance] = analyzer->findClosestEdgeNormalized(person);

        if (closestEdge.has_value()) {
            std::cout << "\nClosest edge: (" << closestEdge->first
                      << "," << closestEdge->second << ")\n";
            std::cout << "Normalized distance: " << distance << "\n";
            auto avg = analyzer->getEdgeAverage(closestEdge->first, closestEdge->second);
            if (avg.has_value()) {
                std::cout << "Average characteristics on this edge:\n  ";
                avg->print();
                std::cout << "\n";
            }

            std::cout << "\nAll distances:\n";
            auto allDistances = analyzer->getAllNormalizedDistances(person);
            for (const auto& [edge, dist] : allDistances) {
                std::cout << "  (" << edge.first << "," << edge.second << "): "
                          << dist << "\n";
            }
        } else {
            std::cout << "Could not find closest edge\n";
        }
    }

    void applyFilters() {
        printHeader("APPLY FILTERS");

        if (allPeople.empty()) {
            std::cout << "No people to filter.\n";
            return;
        }
        int minAge, maxAge;
        std::cout << "Minimum age: "; std::cin >> minAge;
        std::cout << "Maximum age: "; std::cin >> maxAge;

        auto ageFilter = std::make_unique<AgeFilter>(minAge, maxAge);
        auto genderFilter = std::make_unique<GenderFilter>(true);
        auto combined = (*ageFilter) && (*genderFilter);

        std::cout << "\nMales aged " << minAge << "-" << maxAge << ":\n";
        int count = 0;
        for (const auto& p : allPeople) {
            if (combined->passes(*p)) {
                std::cout << "  " << p->name << " " << p->surname
                          << " (" << p->age << " years)\n";
                count++;
            }
        }
        std::cout << "Total: " << count << " people\n";
    }

    void buildRoute() {
        printHeader("BUILD ROUTE");
        if (!analyzer || allPeople.empty()) {
            std::cout << "No data to analyze. Add some people first.\n";
            return;
        }

        std::cout << "Select person from list: ";
        int index;
        std::cin >> index;

        if (index < 1 || index > static_cast<int>(allPeople.size())) {
            std::cout << "Invalid index!\n";
            return;
        }

        const auto& person = allPeople[index-1];
        short maxVertex = getMaxVertex();

        std::cout << "\nSelected: " << person->name << " " << person->surname << "\n";
        std::cout << "Age: " << person->age << ", Weight: " << person->weight
                  << ", Height: " << person->height << "\n\n";
        std::cout << "Starting station (1-" << maxVertex << "): ";
        short start;
        std::cin >> start;

        if (!graph->hasVertex(start)) {
            std::cout << "Station " << start << " does not exist!\n";
            return;
        }
        std::cout << "Number of steps: ";
        int steps;
        std::cin >> steps;
        std::vector<short> path = {start};
        short current = start;
        std::vector<short> previousStation;

        std::cout << "\nBuilding route step by step:\n";

        for (int step = 0; step < steps; ++step) {
            std::cout << "\nStep " << step+1 << " - Current station: " << current << "\n";

            std::vector<std::pair<short, short>> possibleEdges;
            std::vector<double> edgeDistances;
            std::vector<short> possibleTargets;

            for (short neighbor : (*graph)[current]) {
                if (!previousStation.empty() && neighbor == previousStation.back()) {
                    std::cout << "  Skipping " << current << "-" << neighbor
                              << " (would go back to previous station)\n";
                    continue;
                }

                short from = current;
                short to = neighbor;
                if (from > to) std::swap(from, to);

                auto avg = analyzer->getEdgeAverage(from, to);
                if (avg.has_value()) {
                    possibleEdges.emplace_back(current, neighbor);
                    possibleTargets.push_back(neighbor);

                    double dist = analyzer->normalizedEuclideanDistance(*person, from, to);
                    edgeDistances.push_back(dist);

                    std::cout << "  Edge " << current << "-" << neighbor
                              << " (distance: " << dist << ")\n";
                } else {
                    std::cout << "  Edge " << current << "-" << neighbor
                              << " (no data available)\n";
                }
            }

            if (possibleEdges.empty()) {
                std::cout << "  No available edges from station " << current << "\n";
                break;
            }
            int bestIdx = 0;
            for (size_t i = 1; i < edgeDistances.size(); ++i) {
                if (edgeDistances[i] < edgeDistances[bestIdx]) {
                    bestIdx = i;
                }
            }

            short nextStation = possibleTargets[bestIdx];
            std::cout << "  Selected: " << current << " -> " << nextStation
                      << " (distance: " << edgeDistances[bestIdx] << ")\n";

            path.push_back(nextStation);
            previousStation.push_back(current);
            current = nextStation;
        }

        std::cout << "\n" << std::string(40, '-') << "\n";
        std::cout << "FINAL ROUTE (" << path.size()-1 << " steps):\n";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i < path.size() - 1) std::cout << " -> ";
        }
        std::cout << "\n";

        std::cout << "\nSave this route to person? (y/n): ";
        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            std::vector<std::pair<short, short>> routes;
            for (size_t i = 0; i < path.size() - 1; ++i) {
                routes.emplace_back(path[i], path[i + 1]);
            }

            auto updatedPerson = std::make_shared<Person>(
                    person->name, person->surname, person->age, person->isMale,
                    person->isResidense, person->weight, person->height, routes
            );
            for (auto& p : allPeople) {
                if (p == person) {
                    p = updatedPerson;
                    break;
                }
            }
            analyzer = std::make_unique<GraphPersonAnalyzer>(*graph);
            analyzer->addPeople(allPeople);
            analyzer->calculateAverages();
            analyzer->normalize();

            std::cout << "Route saved to person!\n";
        }
    }

    void showNormalizationParams() {
        printHeader("NORMALIZATION PARAMETERS");
        if (!analyzer || allPeople.empty()) {
            std::cout << "No data to normalize.\n";
            return;
        }
        analyzer->getNormalizer().printParams();
    }

    void saveToCSV() {
        printHeader("SAVE TO CSV");

        if (allPeople.empty()) {
            std::cout << "No people to save.\n";
            return;
        }

        std::cout << "Select person to save:\n";
        for (size_t i = 0; i < allPeople.size(); ++i) {
            std::cout << i+1 << ". " << allPeople[i]->name << " "
                      << allPeople[i]->surname << "\n";
        }
        std::cout << "0. Save all\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();
        std::string filename;
        std::cout << "Enter filename: ";
        std::getline(std::cin, filename);
        CSVReader reader(filename);

        if (choice == 0) {
            for (const auto& p : allPeople) {
                reader.writePerson(*p, filename);
            }
            std::cout << "Saved " << allPeople.size() << " people to " << filename << "\n";
        } else if (choice > 0 && choice <= static_cast<int>(allPeople.size())) {
            if (reader.writePerson(*allPeople[choice-1], filename)) {
                std::cout << "Person saved to " << filename << "\n";
            } else {
                std::cout << "Save error\n";
            }
        }
    }

    void showMainMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("GRAPH PERSON ANALYZER");
            std::cout << "1. Show graph\n";
            std::cout << "2. Show all people\n";
            std::cout << "3. Show edge statistics\n";
            std::cout << "4. Find closest edge for a person\n";
            std::cout << "5. Add new person\n";
            std::cout << "6. Apply filters to people\n";
            std::cout << "7. Build route for a person\n";
            std::cout << "8. Show normalization parameters\n";
            std::cout << "9. Load people from CSV\n";
            std::cout << "10. Save people to CSV\n";
            std::cout << "11. Add multiple people manually\n";
            std::cout << "0. Exit\n";
            std::cout << "Choose action: ";

            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            switch(choice) {
                case 1: showGraph(); break;
                case 2: showAllPeople(); break;
                case 3: showEdgeStatistics(); break;
                case 4: findClosestEdge(); break;
                case 5: addPersonManually(); break;
                case 6: applyFilters(); break;
                case 7: buildRoute(); break;
                case 8: showNormalizationParams(); break;
                case 9: loadPeopleFromCSV(); break;
                case 10: saveToCSV(); break;
                case 11: addMultiplePeople(); break;
                case 0: std::cout << "Goodbye!\n"; break;
                default: std::cout << "Invalid choice!\n";
            }
            if (choice != 0) pressEnterToContinue();
        } while (choice != 0);
    }
};

int main() {
    try {
        InteractiveApp app;
        app.showMainMenu();
    } catch (const GraphAnalyzerException& e) {
        std::cerr << "Analyzer error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
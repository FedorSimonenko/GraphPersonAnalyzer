#include "io/CSVReader.hpp"
#include "utils/Exceptions.hpp"
#include <sstream>
#include <algorithm>

namespace graph_analyzer {

CSVReader::CSVReader(const std::string& filename) : filename(filename) {}

std::vector<std::string> CSVReader::parseLine(const std::string& line, char delimeter) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, delimeter)) {
        fields.push_back(field);
    }
    return fields;
}

std::vector<std::pair<short, short>> CSVReader::parseRoutes(const std::string& str) {
    std::vector<std::pair<short, short>> routes;
    std::stringstream ss(str);
    std::string token;
    
    while(std::getline(ss, token, '|')) {
        size_t start = token.find('(');
        size_t end = token.find(')');
        if (start != std::string::npos && end != std::string::npos) {
            std::string route = token.substr(start + 1, end - start - 1);
            auto pos = route.find(';');
            if (pos != std::string::npos) {
                short a = static_cast<short>(std::stoi(route.substr(0, pos)));
                short b = static_cast<short>(std::stoi(route.substr(pos + 1)));
                routes.emplace_back(a, b);
            }
        }
    }
    return routes;
}

Person CSVReader::fromField(const std::vector<std::string>& fields) {
    Person p;
    if (fields.size() >= 8) {
        p.name = fields[0];
        p.surname = fields[1];
        p.age = std::stoul(fields[2]);
        p.isMale = std::stoi(fields[3]) != 0;
        p.isResidense = std::stoi(fields[4]) != 0;
        p.weight = std::stoul(fields[5]);
        p.height = std::stoul(fields[6]);
        p.routes = parseRoutes(fields[7]);
    }
    return p;
}

std::optional<std::vector<Person>> CSVReader::readAll(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return std::nullopt;
    }
    
    std::vector<Person> people;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            auto fields = parseLine(line, ',');
            people.push_back(fromField(fields));
        } catch (const std::exception& e) {
            continue;
        }
    }
    
    return people;
}

std::optional<Person> CSVReader::readOne(const std::string& filename, size_t index) {
    auto all = readAll(filename);
    if (!all.has_value() || index >= all->size()) {
        return std::nullopt;
    }
    return all->at(index);
}

bool CSVReader::writePerson(const Person& person, const std::string& filename) { std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        return false;
    }
    
    file << person.name << ',' << person.surname << ',' << person.age << ',' 
         << person.isMale << ',' << person.isResidense << ',' << person.weight 
         << ',' << person.height << ',';
    
    if (!person.routes.empty()) {
        file << '(' << person.routes[0].first << ';' << person.routes[0].second << ')';
        for (size_t i = 1; i < person.routes.size(); i++) {
            file << "|(" << person.routes[i].first << ';' << person.routes[i].second << ')';
        }
    }
    file << "\n";

    return true;
}
}

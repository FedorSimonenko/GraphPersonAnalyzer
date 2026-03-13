#ifndef CSV_READER_HPP
#define CSV_READER_HPP

#include "ICSVReader.hpp"
#include "utils/Exceptions.hpp"
#include <vector>
#include <fstream>
#include <optional>

namespace graph_analyzer {

class CSVReader : public ICSVReader {
private:
    std::string filename;
    static std::vector<std::string> parseLine(const std::string& line, char delimeter = ',');
    static std::vector<std::pair<short, short>> parseRoutes(const std::string& str);
    static Person fromField(const std::vector<std::string>& fields);

public:
    explicit CSVReader(const std::string& filename);
    
    std::optional<std::vector<Person>> readAll(const std::string& filename) override;
    std::optional<Person> readOne(const std::string& filename, size_t index) override;
    bool writePerson(const Person& person, const std::string& filename) override;
};

}

#endif

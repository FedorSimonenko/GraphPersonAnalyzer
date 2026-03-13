#ifndef ICSV_READER_HPP
#define ICSV_READER_HPP

#include "core/Person.hpp"
#include <string>
#include <vector>
#include <optional>

namespace graph_analyzer {

class ICSVReader {
public:
    virtual ~ICSVReader() = default;
    
    virtual std::optional<std::vector<Person>> readAll(const std::string& filename) = 0;
    virtual std::optional<Person> readOne(const std::string& filename, size_t index) = 0;
    virtual bool writePerson(const Person& person, const std::string& filename) = 0;
};

}

#endif

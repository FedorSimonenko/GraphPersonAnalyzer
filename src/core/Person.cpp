#include "core/Person.hpp"
#include "utils/Exceptions.hpp"

namespace graph_analyzer {

Person::Person(std::string name, std::string surname, size_t age, bool isMale, 
               bool isResidense, size_t weight, size_t height, 
               std::vector<std::pair<short, short>> routes)
    : name(std::move(name))
    , surname(std::move(surname))
    , age(age)
    , isMale(isMale)
    , isResidense(isResidense)
    , weight(weight)
    , height(height)
    , routes(std::move(routes)) {
    
    if (age > Person::getMaxAge()) {
        throw ValidationException("Age exceeds maximum allowed value");
    }
    if (weight > Person::getMaxWeight()) {
        throw ValidationException("Weight exceeds maximum allowed value");
    }
    if (height > Person::getMaxHeight()) {
        throw ValidationException("Height exceeds maximum allowed value");
    }
    if (age < Person::getMinAge()) {
        throw ValidationException("Age below the minimum allowed value");
    }
    if (weight < Person::getMinWeight()) {
        throw ValidationException("Weight below the minimum allowed value");
    }
    if (height < Person::getMinHeight()) {
        throw ValidationException("Height below the minimum allowed value");
    }
}

double Person::getAge() const { return static_cast<double>(age); }
double Person::getWeight() const { return static_cast<double>(weight); }
double Person::getHeight() const { return static_cast<double>(height); }
double Person::getMale() const { return isMale ? 1.0 : 0.0; }
double Person::getResidense() const { return isResidense ? 1.0 : 0.0; }

}

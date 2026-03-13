#ifndef AGE_FILTER_HPP
#define AGE_FILTER_HPP

#include "IFilter.hpp"
#include <memory>
#include <stdexcept>
#include "utils/Exceptions.hpp"

namespace graph_analyzer {

class AgeFilter : public IFilter {
private:
    int minAge;
    int maxAge;
    
public:
    AgeFilter(int min, int max) : minAge(min), maxAge(max) {
        if (min > max) {
            throw ValidationException("Min age cannot be greater than max age");
        }
    }
    
    bool passes(const Person& person) const override {
        return static_cast<int>(person.age) >= minAge && static_cast<int>(person.age) <= maxAge;
    }
    
    std::string getDescription() const override {
        return "Age filter: " + std::to_string(minAge) + " - " + std::to_string(maxAge);
    }
    
    std::unique_ptr<IFilter> clone() const override {
        return std::make_unique<AgeFilter>(*this);
    }
};

}

#endif

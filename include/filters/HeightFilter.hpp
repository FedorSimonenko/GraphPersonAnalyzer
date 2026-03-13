#ifndef HEIGHT_FILTER_HPP
#define HEIGHT_FILTER_HPP

#include "IFilter.hpp"
#include <memory>
#include <stdexcept>
#include "utils/Exceptions.hpp"

namespace graph_analyzer {

class HeightFilter : public IFilter {
private:
    int minHeight;
    int maxHeight;
    
public:
    HeightFilter(int min, int max) : minHeight(min), maxHeight(max) {
        if (min > max) {
            throw ValidationException("Min height cannot be greater than max height");
        }
    }
    
    bool passes(const Person& person) const override {
        return static_cast<int>(person.height) >= minHeight && static_cast<int>(person.height) <= maxHeight;
    }
    
    std::string getDescription() const override {
        return "Height filter: " + std::to_string(minHeight) + " - " + std::to_string(maxHeight);
    }
    
    std::unique_ptr<IFilter> clone() const override {
        return std::make_unique<HeightFilter>(*this);
    }
};

}

#endif

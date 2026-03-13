#ifndef WEIGHT_FILTER_HPP
#define WEIGHT_FILTER_HPP

#include "IFilter.hpp"
#include <memory>
#include <stdexcept>
#include "utils/Exceptions.hpp"

namespace graph_analyzer {

class WeightFilter : public IFilter {
private:
    int minWeight;
    int maxWeight;
    
public:
    WeightFilter(int min, int max) : minWeight(min), maxWeight(max) {
        if (min > max) {
            throw ValidationException("Min weight cannot be greater than max weight");
        }
    }
    
    bool passes(const Person& person) const override {
        return static_cast<int>(person.weight) >= minWeight && static_cast<int>(person.weight) <= maxWeight;
    }
    
    std::string getDescription() const override {
        return "Weight filter: " + std::to_string(minWeight) + " - " + std::to_string(maxWeight);
    }
    
    std::unique_ptr<IFilter> clone() const override {
        return std::make_unique<WeightFilter>(*this);
    }
};

}

#endif

#ifndef RESIDENCE_FILTER_HPP
#define RESIDENCE_FILTER_HPP

#include "IFilter.hpp"
#include <memory>

namespace graph_analyzer {

class ResidenceFilter : public IFilter {
private:
    bool targetResidence;
    
public:
    explicit ResidenceFilter(bool residence) : targetResidence(residence) {}
    
    bool passes(const Person& person) const override {
        return person.isResidense == targetResidence;
    }
    
    std::string getDescription() const override {
        return "Residence filter: " + std::string(targetResidence ? "Local" : "Non-local");
    }
    
    std::unique_ptr<IFilter> clone() const override {
        return std::make_unique<ResidenceFilter>(*this);
    }
};

}

#endif

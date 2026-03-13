#ifndef GENDER_FILTER_HPP
#define GENDER_FILTER_HPP

#include "IFilter.hpp"
#include <memory>

namespace graph_analyzer {

class GenderFilter : public IFilter {
private:
    bool targetGender;
    
public:
    explicit GenderFilter(bool male) : targetGender(male) {}
    
    bool passes(const Person& person) const override {
        return person.isMale == targetGender;
    }
    
    std::string getDescription() const override {
        return "Gender filter: " + std::string(targetGender ? "Male" : "Female");
    }
    
    std::unique_ptr<IFilter> clone() const override {
        return std::make_unique<GenderFilter>(*this);
    }
};

}

#endif

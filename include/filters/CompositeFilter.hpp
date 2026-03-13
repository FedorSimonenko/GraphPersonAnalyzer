#ifndef COMPOSITE_FILTER_HPP
#define COMPOSITE_FILTER_HPP

#include "IFilter.hpp"
#include <memory>
#include <vector>
#include <sstream>

namespace graph_analyzer {

class CompositeFilter : public IFilter {
private:
    std::vector<std::unique_ptr<IFilter>> filters;
    bool isAnd;
    
public:
    explicit CompositeFilter(bool useAnd = true) : isAnd(useAnd) {}
    
    void addFilter(std::unique_ptr<IFilter> filter) {
        if (filter) {
            filters.push_back(std::move(filter));
        }
    }
    
    void addFilterShared(std::shared_ptr<IFilter> filter) {
        if (filter) {
            filters.push_back(filter->clone());
        }
    }
    
    bool passes(const Person& person) const override {
        if (filters.empty()) return true;
        
        if (isAnd) {
            for (const auto& filter : filters) {
                if (!filter->passes(person)) return false;
            }
            return true;
        } else {
            for (const auto& filter : filters) {
                if (filter->passes(person)) return true;
            }
            return false;
        }
    }
    
    std::string getDescription() const override {
        std::ostringstream oss;
        oss << (isAnd ? "AND" : "OR") << " Composite (";
        for (size_t i = 0; i < filters.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << filters[i]->getDescription();
        }
        oss << ")";
        return oss.str();
    }
    
    std::unique_ptr<IFilter> clone() const override {
        auto composite = std::make_unique<CompositeFilter>(isAnd);
        for (const auto& filter : filters) {
            composite->addFilter(filter->clone());
        }
        return composite;
    }
};

}

#endif

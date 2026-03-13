#ifndef IANALYZER_HPP
#define IANALYZER_HPP

#include <vector>
#include <optional>
#include "Person.hpp"

namespace graph_analyzer {

class IAnalyzer {
public:
    virtual ~IAnalyzer() = default;
    
    virtual std::optional<double> analyze(const Person& person) const = 0;
    virtual std::string getName() const = 0;
    virtual bool isInitialized() const = 0;
    
    virtual std::vector<double> analyzeBatch(const std::vector<Person>& people) const {
        std::vector<double> results;
        for (const auto& p : people) {
            auto result = analyze(p);
            if (result.has_value()) {
                results.push_back(result.value());
            }
        }
        return results;
    }
    
    static constexpr double getDefaultThreshold() { return 0.5; }
};

}

#endif

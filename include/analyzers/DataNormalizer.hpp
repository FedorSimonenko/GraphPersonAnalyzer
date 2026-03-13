#ifndef DATA_NORMALIZER_HPP
#define DATA_NORMALIZER_HPP

#include "core/Person.hpp"
#include "EdgeAverage.hpp"
#include "core/IAnalyzer.hpp"
#include <vector>
#include <unordered_map>
#include <limits>
#include <iostream>

namespace graph_analyzer {

class DataNormalizer : public IAnalyzer {
private:
    struct NormalizationParams {
        double minVal = std::numeric_limits<double>::max();
        double maxVal = std::numeric_limits<double>::lowest();
        
        void update(double val);
        double normalize(double val) const;
    };
    
    std::unordered_map<std::string, NormalizationParams> params;
    bool isFitted = false;

public:
    DataNormalizer() = default;
    
    void fit(const std::vector<Person>& people);
    void fitFromAverages(const std::vector<EdgeAverage>& averages);
    
    std::vector<double> normalizePerson(const Person& p) const;
    std::vector<double> normalizeEdgeAverage(const EdgeAverage& avg) const;
    
    std::optional<double> analyze(const Person& person) const override;
    std::string getName() const override {return "DataNormalizer";}
    bool isInitialized() const override {return isFitted;}
    
    void printParams() const;
};

}

#endif

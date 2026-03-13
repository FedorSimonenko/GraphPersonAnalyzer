#include "analyzers/DataNormalizer.hpp"
#include "utils/Exceptions.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace graph_analyzer {

void DataNormalizer::NormalizationParams::update(double val) {
    minVal = std::min(minVal, val);
    maxVal = std::max(maxVal, val);
}

double DataNormalizer::NormalizationParams::normalize(double val) const {
    if (maxVal - minVal < 1e-10) return 0.5;
    return (val - minVal) / (maxVal - minVal);
}

void DataNormalizer::fit(const std::vector<Person>& people) {
    if (people.empty()) {
        throw NormalizationException("Cannot fit on empty dataset");
    }
    for (const auto& p : people) {
        params["age"].update(p.getAge());
        params["weight"].update(p.getWeight());
        params["height"].update(p.getHeight());
        params["male"].update(p.getMale());
        params["residense"].update(p.getResidense());
    }
    isFitted = true;
}

void DataNormalizer::fitFromAverages(const std::vector<EdgeAverage>& averages) {
    if (averages.empty()) return;
    for (const auto& avg : averages) {
        params["age"].update(avg.avgAge);
        params["weight"].update(avg.avgWeight);
        params["height"].update(avg.avgHeight);
        params["male"].update(avg.avgMale);
        params["residense"].update(avg.avgResidense);
    }
    isFitted = true;
}

std::vector<double> DataNormalizer::normalizePerson(const Person& p) const {
    if (!isFitted) {
        throw NormalizationException("Normalizer not fitted. Call fit() first.");
    }
    
    return {
        params.at("age").normalize(p.getAge()),
        params.at("weight").normalize(p.getWeight()),
        params.at("height").normalize(p.getHeight()),
        params.at("male").normalize(p.getMale()),
        params.at("residense").normalize(p.getResidense())
    };
}

std::vector<double> DataNormalizer::normalizeEdgeAverage(const EdgeAverage& avg) const {
    if (!isFitted) {
        throw NormalizationException("Normalizer not fitted. Call fit() first.");
    }
    
    return {
        params.at("age").normalize(avg.avgAge),
        params.at("weight").normalize(avg.avgWeight),
        params.at("height").normalize(avg.avgHeight),
        params.at("male").normalize(avg.avgMale),
        params.at("residense").normalize(avg.avgResidense)
    };
}

std::optional<double> DataNormalizer::analyze(const Person& person) const {
    if (!isFitted) return std::nullopt;
    auto norm = normalizePerson(person);
    double sum = 0;
    for (double v : norm) sum += v;
    return sum / norm.size();
}

void DataNormalizer::printParams() const {
    std::cout << "\n=== Parameters maximum and minimum ===" << std::endl;
    for (const auto& [name, param] : params) {
        std::cout << name << ": min=" << param.minVal 
                  << ", max=" << param.maxVal << std::endl;
    }
}

}

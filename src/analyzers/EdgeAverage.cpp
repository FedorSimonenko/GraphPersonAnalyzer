#include "analyzers/EdgeAverage.hpp"
#include <cmath>
#include <iostream>

namespace graph_analyzer {

EdgeAverage::EdgeAverage(const std::vector<std::shared_ptr<const Person>>& people) {
    if (people.empty()) return;
    
    double sumAge = 0, sumWeight = 0, sumHeight = 0;
    double sumMale = 0, sumResidense = 0;
    
    for (const auto& p : people) {
        sumAge += p->getAge();
        sumWeight += p->getWeight();
        sumHeight += p->getHeight();
        sumMale += p->isMale ? 1.0 : 0.0;
        sumResidense += p->isResidense ? 1.0 : 0.0;
    }
    
    size_t count = people.size();
    avgAge = sumAge / count;
    avgWeight = sumWeight / count;
    avgHeight = sumHeight / count;
    avgMale = sumMale / count;
    avgResidense = sumResidense / count;
}

EdgeAverage::EdgeAverage(const std::vector<const Person*>& people) {
    if (people.empty()) return;
    
    double sumAge = 0, sumWeight = 0, sumHeight = 0;
    double sumMale = 0, sumResidense = 0;
    
    for (const auto* p : people) {
        sumAge += p->getAge();
        sumWeight += p->getWeight();
        sumHeight += p->getHeight();
        sumMale += p->isMale ? 1.0 : 0.0;
        sumResidense += p->isResidense ? 1.0 : 0.0;
    }
    
    size_t count = people.size();
    avgAge = sumAge / count;
    avgWeight = sumWeight / count;
    avgHeight = sumHeight / count;
    avgMale = sumMale / count;
    avgResidense = sumResidense / count;
}

void EdgeAverage::print() const {
    std::cout << "Avg Age: " << avgAge 
              << ", Avg Weight: " << avgWeight
              << ", Avg Height: " << avgHeight
              << ", Male ratio: " << avgMale
              << ", Residense ratio: " << avgResidense;
}

bool EdgeAverage::operator==(const EdgeAverage& other) const {
    const double eps = 1e-6;
    return std::abs(avgAge - other.avgAge) < eps &&
           std::abs(avgWeight - other.avgWeight) < eps &&
           std::abs(avgHeight - other.avgHeight) < eps &&
           std::abs(avgMale - other.avgMale) < eps &&
           std::abs(avgResidense - other.avgResidense) < eps;
}

}

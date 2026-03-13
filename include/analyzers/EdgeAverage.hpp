#ifndef EDGE_AVERAGE_HPP
#define EDGE_AVERAGE_HPP

#include "core/Person.hpp"
#include <vector>
#include <iostream>
#include <memory>

namespace graph_analyzer {

struct EdgeAverage {
    double avgAge = 0.0;
    double avgWeight = 0.0;
    double avgHeight = 0.0;
    double avgMale = 0.0;
    double avgResidense = 0.0;
    
    EdgeAverage() = default;
    explicit EdgeAverage(const std::vector<std::shared_ptr<const Person>>& people);
    explicit EdgeAverage(const std::vector<const Person*>& people);
    
    void print() const;
    
    static constexpr size_t getDimension() {return 5;}

    bool operator==(const EdgeAverage& other) const;
};

}

#endif

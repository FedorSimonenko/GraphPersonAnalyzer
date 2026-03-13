#ifndef IFILTER_HPP
#define IFILTER_HPP

#include <memory>
#include <vector>
#include "core/Person.hpp"

namespace graph_analyzer {

class IFilter {
public:
    virtual ~IFilter() = default;
    
    virtual bool passes(const Person& person) const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::unique_ptr<IFilter> clone() const = 0;
    
    virtual std::unique_ptr<IFilter> operator&&(const IFilter& other) const;
    virtual std::unique_ptr<IFilter> operator||(const IFilter& other) const;
};

}

#endif

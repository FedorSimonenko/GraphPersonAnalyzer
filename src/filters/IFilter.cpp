#include "filters/IFilter.hpp"
#include "filters/CompositeFilter.hpp"

namespace graph_analyzer {

std::unique_ptr<IFilter> IFilter::operator&&(const IFilter& other) const {
    auto composite = std::make_unique<CompositeFilter>(true);
    composite->addFilter(this->clone());
    composite->addFilter(other.clone());
    return composite;
}

std::unique_ptr<IFilter> IFilter::operator||(const IFilter& other) const {
    auto composite = std::make_unique<CompositeFilter>(false);
    composite->addFilter(this->clone());
    composite->addFilter(other.clone());
    return composite;
}

}

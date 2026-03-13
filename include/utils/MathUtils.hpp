#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <vector>
#include <cmath>
#include <numeric>
#include <optional>
#include <limits>

namespace graph_analyzer {

class MathUtils {
public:
    static constexpr double squared(double x) { return x * x; }
    
    static constexpr double degreesToRadians(double degrees) {
        return degrees * 3.14159265358979323846 / 180.0;
    }
    
    static constexpr double radiansToDegrees(double radians) {
        return radians * 180.0 / 3.14159265358979323846;
    }
    
    template<typename T>
    static std::optional<double> mean(const std::vector<T>& values) {
        if (values.empty()) return std::nullopt;
        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        return sum / values.size();
    }
    
    template<typename T>
    static std::optional<double> variance(const std::vector<T>& values) {
        auto meanOpt = mean(values);
        if (!meanOpt.has_value()) return std::nullopt;
        double meanVal = meanOpt.value();
        double sum = 0.0;
        for (const auto& v : values) {
            sum += squared(static_cast<double>(v) - meanVal);
        }
        return sum / values.size();
    }
    
    static double euclideanDistance(const std::vector<double>& v1, const std::vector<double>& v2) {
        if (v1.size() != v2.size()) return std::numeric_limits<double>::max();
        double sum = 0.0;
        for (size_t i = 0; i < v1.size(); ++i) {
            sum += squared(v1[i] - v2[i]);
        }
        return std::sqrt(sum);
    }
};

}

#endif

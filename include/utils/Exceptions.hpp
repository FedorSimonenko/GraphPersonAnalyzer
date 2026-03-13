#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

namespace graph_analyzer {

class GraphAnalyzerException : public std::runtime_error {
public:
    explicit GraphAnalyzerException(const std::string& message) 
        : std::runtime_error(message) {}
};

class GraphException : public GraphAnalyzerException {
public:
    explicit GraphException(const std::string& message) 
        : GraphAnalyzerException("Graph error: " + message) {}
};

class ValidationException : public GraphAnalyzerException {
public:
    explicit ValidationException(const std::string& message) 
        : GraphAnalyzerException("Validation error: " + message) {}
};

class FileException : public GraphAnalyzerException {
public:
    explicit FileException(const std::string& message) 
        : GraphAnalyzerException("File error: " + message) {}
};

class NormalizationException : public GraphAnalyzerException {
public:
    explicit NormalizationException(const std::string& message) 
        : GraphAnalyzerException("Normalization error: " + message) {}
};

}

#endif

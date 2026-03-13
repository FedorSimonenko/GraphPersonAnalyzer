
add_library(graph_analyzer_core
        ${CMAKE_CURRENT_SOURCE_DIR}/core/Graph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/Person.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/IAnalyzer.cpp
)

add_library(graph_analyzer_filters
        ${CMAKE_CURRENT_SOURCE_DIR}/filters/IFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filters/AgeFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filters/WeightFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filters/HeightFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filters/GenderFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filters/ResidenceFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filters/CompositeFilter.cpp
)

add_library(graph_analyzer_analyzers
        ${CMAKE_CURRENT_SOURCE_DIR}/analyzers/EdgeAverage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/analyzers/DataNormalizer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/analyzers/GraphPersonAnalyzer.cpp
)

add_library(graph_analyzer_io
        ${CMAKE_CURRENT_SOURCE_DIR}/io/CSVReader.cpp
)

add_library(graph_analyzer_utils
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/Exceptions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/MathUtils.cpp
)

target_include_directories(graph_analyzer_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include/core
)

target_include_directories(graph_analyzer_filters PUBLIC
        ${CMAKE_SOURCE_DIR}/include/filters
)

target_include_directories(graph_analyzer_analyzers PUBLIC
        ${CMAKE_SOURCE_DIR}/include/analyzers
)

target_include_directories(graph_analyzer_io PUBLIC
        ${CMAKE_SOURCE_DIR}/include/io
)

target_include_directories(graph_analyzer_utils PUBLIC
        ${CMAKE_SOURCE_DIR}/include/utils
)

target_link_libraries(graph_analyzer_filters PUBLIC
        graph_analyzer_core
        graph_analyzer_utils
)

target_link_libraries(graph_analyzer_analyzers PUBLIC
        graph_analyzer_core
        graph_analyzer_utils
        graph_analyzer_filters
)

target_link_libraries(graph_analyzer_io PUBLIC
        graph_analyzer_core
        graph_analyzer_utils
)

add_library(graph_analyzer_all INTERFACE)
target_link_libraries(graph_analyzer_all INTERFACE
        graph_analyzer_core
        graph_analyzer_filters
        graph_analyzer_analyzers
        graph_analyzer_io
        graph_analyzer_utils
)
cmake_minimum_required(VERSION 3.28)
project(GraphPersonAnalyzer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_APP "Build application" ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)
    include_directories(${gtest_SOURCE_DIR}/include)
endif()

if(MSVC)
    add_compile_options(/W4 /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
file(GLOB_RECURSE LIB_HEADERS "include/*.hpp")

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES} ${LIB_HEADERS})

if(BUILD_APP)
    add_subdirectory(app)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
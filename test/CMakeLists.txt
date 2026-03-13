add_executable(runGlobalTests
        test_global.cpp
)

target_link_libraries(runGlobalTests
        PRIVATE
        GraphPersonAnalyzer
        gtest
        gtest_main
)

add_test(NAME GlobalTests COMMAND runGlobalTests)
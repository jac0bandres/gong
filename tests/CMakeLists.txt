add_executable(gong-tests
    test_local_storage.cpp
)

target_link_libraries(gong-tests
    PRIVATE
        gong-core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gong-tests)
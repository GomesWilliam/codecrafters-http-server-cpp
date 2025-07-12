include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(main_test main_test.cpp)
set_target_properties(main_test PROPERTIES EXCLUDE_FROM_ALL FALSE)
target_link_libraries(main_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(main_test
        DISCOVERY_MODE PRE_TEST
        PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        DISCOVERY_TIMEOUT 30
)

add_custom_target(run_tests ALL DEPENDS main_test)  # 🔥 Ensures main_test is built
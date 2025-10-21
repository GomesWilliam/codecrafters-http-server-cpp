cmake_minimum_required(VERSION 3.30)
project(http-server-starter-cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

# Main executable
add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE Threads::Threads ZLIB::ZLIB)

# Optional testing
option(BUILD_TESTING "Enable tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
    add_dependencies(server main_test)
endif()
cmake_minimum_required(VERSION 3.30)
project(http-server-starter-cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CLANG_TIDY_ARGS "" CACHE STRING "Extra clang-tidy arguments (semicolon-separated)")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

# ---- library with your core code ----
add_library(http_server
  src/request.cpp
  include/request.h
)

target_include_directories(http_server PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(http_server PUBLIC Threads::Threads ZLIB::ZLIB)

# Warnings: strict in Debug, normal in Release
target_compile_options(http_server PRIVATE
  $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic -Werror>
  $<$<CONFIG:Release>:-Wall -Wextra -Wpedantic>
)

# ---- main executable ----
add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE http_server)

# ---- optional testing ----
option(BUILD_TESTING "Enable tests" ON)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

cmake_minimum_required(VERSION 3.30)
project(http-server-starter-cpp)

# clang-tidy auto (honors your .clang-tidy)
find_program(CLANG_TIDY_EXE NAMES 
    clang-tidy 
    clang-tidy-17 
    clang-tidy-16 
    clang-tidy-15
)
if(CLANG_TIDY_EXE)
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-p=${CMAKE_BINARY_DIR}")
else()
  message(WARNING "clang-tidy not found!")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

# ---- library with your core code ----
add_library(http_server
    src/request.cpp
    include/request.h
)

target_include_directories(http_server
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(http_server
    PUBLIC
        Threads::Threads
        ZLIB::ZLIB
)

# Warnings: strict in Debug, normal in Release
target_compile_options(http_server
    PRIVATE
        $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CONFIG:Release>:-Wall -Wextra -Wpedantic>
)

# ---- main executable ----
add_executable(server
    src/server.cpp
)

target_link_libraries(server
    PRIVATE http_server
)

# ---- optional testing ----
option(BUILD_TESTING "Enable tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

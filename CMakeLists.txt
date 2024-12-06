cmake_minimum_required(VERSION 3.21)
project(xgraph VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

add_subdirectory("${CMAKE_SOURCE_DIR}/3rdparty/catch2")

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME}_tests PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(${PROJECT_NAME}_tests PRIVATE Catch2::Catch2WithMain)

cmake_minimum_required(VERSION 3.21)
project(xgraph VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)

include_directories("${CMAKE_SOURCE_DIR}/include")

# Add subdirectory for the tests directory
add_subdirectory(tests)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include/")

cmake_minimum_required(VERSION 3.21)
project(xgraph VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY  https://github.com/google/googletest.git
        GIT_TAG         b514bdc898e2951020cbdca1304b75f5950d1f59 # release-1.15.2
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")
foreach (TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})

    target_include_directories(${TEST_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include/")
    target_link_libraries(${TEST_NAME} GTest::gtest_main)

    gtest_discover_tests(${TEST_NAME})
endforeach ()

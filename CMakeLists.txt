cmake_minimum_required(VERSION 3.30)
project(xgraph VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(xgraph INTERFACE)
add_library(xgraph::xgraph ALIAS xgraph)

target_include_directories(xgraph INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/xgraph>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Build test
option(XGRAPH_BUILD_TESTS "Build tests" OFF)

if(XGRAPH_BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.10.0
    )

    FetchContent_MakeAvailable(Catch2)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

    include(CTest)
    include(Catch)

    add_subdirectory(tests)
endif()

# Build doc
option(XGRAPH_BUILD_DOC "Build documentation" OFF)

if(XGRAPH_BUILD_DOC)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN  ${CMAKE_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        message(STATUS "Doxygen build enabled")

        add_custom_target(doc_doxygen
          COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
          COMMENT "Generating API documentation with Doxygen"
          VERBATIM
        )
    else()
        message(WARNING "Doxygen not found, documentation will not be built")
    endif()
endif()

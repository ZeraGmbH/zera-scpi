cmake_minimum_required(VERSION 3.1)

project(zera-scpi LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SCPI_VERSION_MAJOR "2")
set(SCPI_VERSION_MINOR "0")
set(SCPI_VERSION_PATCH "3")

set(PROJECT_VERSION "${SCPI_VERSION_MAJOR}.${SCPI_VERSION_MINOR}.${SCPI_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(Qt5 COMPONENTS Core Xml Test CONFIG REQUIRED)
find_package(xiqnet REQUIRED)

enable_testing()

# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt

function(SETUP_TESTS)
    foreach(_testname ${ARGN})
        add_test(NAME ${_testname}
                 COMMAND ${_testname})
        add_executable(${_testname}
            ${_testname}.h
            ${_testname}.cpp
            )
        target_include_directories(${_testname}
            PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            ${PROJECT_SOURCE_DIR}/src
            )
        target_link_libraries(${_testname}
            PUBLIC
            Qt5::Test
            scpi-testlib
            xml-compare-testlib
            scpi-tcp
            )
    endforeach()
endfunction()

add_subdirectory(scpi)
add_subdirectory(scpi-tcp)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

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
add_subdirectory(scpi)
add_subdirectory(scpi-tcp)
add_subdirectory(tests)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

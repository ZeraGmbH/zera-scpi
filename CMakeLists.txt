cmake_minimum_required(VERSION 3.1)

#set project name and language
project(vf-helpers LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#C++ 11 Flag
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Set project Version
set(SCPI_VERSION_MAJOR "2")
set(SCPI_VERSION_MINOR "0")
set(SCPI_VERSION_PATCH "3")

set(PROJECT_VERSION "${SCPI_VERSION_MAJOR}.${SCPI_VERSION_MINOR}.${SCPI_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

#Find dependecies
find_package(Qt5 COMPONENTS Core Xml Gui CONFIG REQUIRED)

#sum up project Files 
file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )

#create library 
add_library(SCPI SHARED
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    )


#link dependencies
target_link_libraries(SCPI 
    PRIVATE
    Qt5::Core
    Qt5::Xml
    Qt5::Gui
    )

# announce headers - target perspective
target_include_directories(SCPI 
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SCPI>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#set target Version
set_target_properties(SCPI PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(SCPI PROPERTIES SOVERSION ${SCPI_VERSION_MAJOR})

#generate export header
include(GenerateExportHeader)
generate_export_header(SCPI)



#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/scpi_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SCPI
    )




# install library
install(TARGETS SCPI
    EXPORT SCPIExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SCPIConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )

# configure *Config.cmake
configure_file(SCPIConfig.cmake.in SCPIConfig.cmake @ONLY)


# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SCPIConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SCPIConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SCPI
    )

# install targets cmake-files
install(EXPORT SCPIExport
    FILE SCPITargets.cmake
    NAMESPACE Zera::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SCPI
    )

# install public headers
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SCPI
    )

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)






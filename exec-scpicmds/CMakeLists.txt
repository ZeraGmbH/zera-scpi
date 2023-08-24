file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

file(GLOB SOURCES_NODETREE
    ${CMAKE_CURRENT_SOURCE_DIR}/nodetree/*.cpp
    )

file(GLOB HEADER_NODETREE
    ${CMAKE_CURRENT_SOURCE_DIR}/nodetree/*.h
    )


add_library(execScpiCmdslib STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(execScpiCmdslib
    PUBLIC
    Qt5::Core
    Qt5::Network
    )

target_include_directories(execScpiCmdslib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/nodetree>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/execScpiCmdslib>
    )

set_target_properties(execScpiCmdslib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(execScpiCmdslib PROPERTIES SOVERSION ${SCPI_VERSION_MAJOR})


add_library(execScpiCmdsNodeTreelib STATIC
    ${SOURCES_NODETREE}
    ${HEADER_NODETREE}
    )

target_link_libraries(execScpiCmdsNodeTreelib
    PUBLIC
    Qt5::Core
    )

target_include_directories(execScpiCmdsNodeTreelib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/nodetree>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/execScpiCmdsNodeTreelib>
    )

set_target_properties(execScpiCmdsNodeTreelib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(execScpiCmdsNodeTreelib PROPERTIES SOVERSION ${SCPI_VERSION_MAJOR})


# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    execScpiCmdslibConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

install(TARGETS execScpiCmdslib
    EXPORT execScpiCmdslib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/execScpiCmdslib
)

add_executable(execScpiCmds
    main.cpp
)

target_link_libraries(execScpiCmds
    PUBLIC
    execScpiCmdslib
    execScpiCmdsNodeTreelib
)

install(TARGETS execScpiCmds
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

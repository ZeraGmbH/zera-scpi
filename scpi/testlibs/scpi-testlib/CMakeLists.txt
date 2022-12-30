file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(scpi-testlib STATIC
    ${SOURCES}
    ${HEADER}
)

target_include_directories(scpi-testlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/scpi-testlib>
    )

target_link_libraries(scpi-testlib
    PUBLIC
    Qt5::Core
    SCPI
)

set_target_properties(scpi-testlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(scpi-testlib PROPERTIES SOVERSION ${SCPI_VERSION_MAJOR})

include(GenerateExportHeader)
generate_export_header(scpi-testlib)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/scpi-testlib_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/scpi-testlib
    )

install(TARGETS scpi-testlib
    EXPORT SCPITestLibExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

write_basic_package_version_file(
    SCPITestlibConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(SCPITestlibConfig.cmake.in SCPITestlibConfig.cmake @ONLY)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SCPITestlibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SCPITestlibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SCPITestlib
    )

install(EXPORT SCPITestLibExport
    FILE SCPITestlibTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SCPITestlib
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/scpi-testlib
    )

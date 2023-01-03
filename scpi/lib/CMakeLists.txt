file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_library(SCPI SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(SCPI
    PUBLIC
    Qt5::Core
    Qt5::Xml
    )

target_include_directories(SCPI
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SCPI>
    )

set_target_properties(SCPI PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(SCPI PROPERTIES SOVERSION ${SCPI_VERSION_MAJOR})

generate_export_header(SCPI)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/scpi_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SCPI
    )

export(TARGETS SCPI NAMESPACE Zera:: FILE SCPITargets.cmake)
install(TARGETS SCPI
    EXPORT SCPIExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SCPI
    )

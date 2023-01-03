file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_library(scpi-tcp SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(scpi-tcp
    PUBLIC
    Qt5::Core
    SCPI
    VeinMeta::xiqnet 
    )

# announce headers - target perspective
target_include_directories(scpi-tcp
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/scpi-tcp>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

set_target_properties(scpi-tcp PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(scpi-tcp PROPERTIES SOVERSION ${SCPI_VERSION_MAJOR})

generate_export_header(scpi-tcp)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/scpi-tcp_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/scpi-tcp
    )

install(TARGETS scpi-tcp
    EXPORT SCPIExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/scpi-tcp
    )

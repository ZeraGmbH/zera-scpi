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
    )

target_link_libraries(scpi-testlib
    PUBLIC
    Qt5::Core
)


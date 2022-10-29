# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt

add_library(testhelpers STATIC
    xmlcomparer.cpp
    xmlcomparer.h
    xmlelemcompareattribs.cpp
    xmlelemcompareattribs.h
    xmldocument.cpp
    xmldocument.h
    xmlelemiterator.cpp
    xmlelemiterator.h
    xmlelemcomparetext.cpp
    xmlelemcomparetext.h
    )

target_include_directories(testhelpers
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(testhelpers
    PUBLIC
    Qt5::Core
    Qt5::Xml
    )

enable_testing()

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
            testhelpers
            scpi-tcp
            )
    endforeach()
endfunction()

SETUP_TESTS(
    test_scpisingletonfactory
    test_xmlcomparer
    test_xmldocument
    test_xmlelemcompareattribs
    test_xmlelemcomparetext
    test_xmlelemiterator
)

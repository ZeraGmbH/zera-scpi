# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt

add_library(helpers_xml STATIC
    xmlqtdomdociteration.cpp
    xmlqtdomdociteration.h
    xmldocumentcompare.cpp
    xmldocumentcompare.h
    xmlelemcompareattribs.cpp
    xmlelemcompareattribs.h
    xmlelemcomparetext.cpp
    xmlelemcomparetag.h
    xmlelemcomparetag.cpp
    xmlelemcomparetext.h
    xmlelemiter.cpp
    xmlelemiter.h
    xmlelemiterstrategylist.cpp
    xmlelemiterstrategylist.h
    xmlelemiterstrategysort.cpp
    xmlelemiterstrategysort.h
    xmlelemiterstrategytree.cpp
    xmlelemiterstrategytree.h
    )

target_include_directories(helpers_xml
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(helpers_xml
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
            helpers_xml
            scpi-tcp
            )
    endforeach()
endfunction()

SETUP_TESTS(
    test_scpisingletonfactory
    test_scpiinterface
    test_scpiinterfacexml
    test_xmldocumentcompare
    test_xmldocument
    test_xmlelemcompareattribs
    test_xmlelemcomparetext
    test_xmlelemiter
    test_xmlelemiteratorlist
    test_xmlelemiteratorsort
    test_xmlelemiteratortree
)

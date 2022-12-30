add_library(helpers_xml STATIC
    xmldocument.cpp
    xmldocument.h
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



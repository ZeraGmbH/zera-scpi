add_library(xml-compare-testlib STATIC
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

target_include_directories(xml-compare-testlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(xml-compare-testlib
    PUBLIC
    Qt5::Core
    Qt5::Xml
    )

export(TARGETS xml-compare-testlib NAMESPACE Zera:: FILE SCPITargets.cmake)
install(TARGETS xml-compare-testlib
    EXPORT SCPIExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xml-compare-testlib
    )


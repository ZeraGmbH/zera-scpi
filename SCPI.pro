#-------------------------------------------------
#
# Project created by QtCreator 2012-07-17T10:15:23
#
#-------------------------------------------------

TARGET = SCPI
TEMPLATE = lib
QT           += xml

VERSION = 2.0.0

DEFINES += SCPI_LIBRARY

SOURCES += scpi.cpp \
    parse.cpp \
    scpi_p.cpp \
    scpistring.cpp \
    scpiobject.cpp \
    scpinode.cpp \
    parse_p.cpp \
    scpicommand.cpp \
    scpicommand_p.cpp

HEADERS += scpi.h\
        SCPI_global.h \
    parse.h \
    scpi_p.h \
    scpistring.h \
    scpiobject.h \
    scpiobject_p.h \
    scpinode.h \
    parse_p.h \
    scpicommand.h \
    scpicommand_p.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


header_files.files = $$HEADERS
header_files.path = /usr/include/scpi
INSTALLS += header_files

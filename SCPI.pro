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

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE0DCD3E9
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = SCPI.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

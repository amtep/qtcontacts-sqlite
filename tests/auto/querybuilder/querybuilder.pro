TARGET = tst_querybuilder
include(../../common.pri)

QT += contacts-private

INCLUDEPATH += \
    ../../../src/engine/

HEADERS += \
    ../../../src/engine/querybuilder_p.h \
    ../../util.h

SOURCES += \
    ../../../src/engine/querybuilder_p.cpp \
    tst_querybuilder.cpp

HEADERS += testrecover.h \
           tosource.h \
           filehandler.h \
           filepreprocessor.h
SOURCES += testrecover.cpp \
           tosource.cpp \
           filehandler.cpp \
           filepreprocessor.cpp

DEBUGGER += main.cpp

INCLUDEPATH += ../parser ../parser/rpp ../duchain ../duchain/editor ../duchain/util
LIBS += -L../parser -lparser -L../duchain -lduchain

OBJECTS_DIR = tmp
MOC_DIR = tmp

CONFIG += debug \
warn_on \
qt \
console \
qtestlib \
silent
TEMPLATE = app

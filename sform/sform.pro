HEADERS += sform.h \
           code.h 
SOURCES += main.cpp \
           sform.cpp \
           code.cpp

OBJECTS_DIR = tmp
MOC_DIR = tmp

TEMPLATE = app
TARGET = sform
RESOURCES = sform.qrc
CONFIG += qt debug warn_on console

QMAKE_CXXFLAGS += -O0 -fdump-translation-unit-all

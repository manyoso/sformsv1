HEADERS += sform.h
SOURCES += sform.cpp

OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

TEMPLATE = app
TARGET = sform
RESOURCES = sform.qrc
CONFIG += qt debug warn_on console

;QMAKE_CXXFLAGS += -S

include($$PWD/../../sforms.pri)

HEADERS +=
SOURCES += main.cpp
INCLUDEPATH += $$PWD/../../diff/lib/
LIBS += -lQtDiff

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = test
CONFIG += qt
QT += core

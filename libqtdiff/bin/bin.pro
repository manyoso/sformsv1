include($$PWD/../../sforms.pri)

HEADERS +=
SOURCES += main.cpp
INCLUDEPATH += $$PWD/../lib/
LIBS += -lQtDiff

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = qtdiff
CONFIG += qt
QT += core

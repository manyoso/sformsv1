include($$PWD/../sforms.pri)

HEADERS += sform.h
SOURCES += sform.cpp

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = sform
CONFIG += qt
QT += core network

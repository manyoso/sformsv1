include($$PWD/../sforms.pri)

HEADERS += control.h
SOURCES += main.cpp \
           control.cpp

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = control
CONFIG += qt
QT += core gui network

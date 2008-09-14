include($$PWD/../sforms.pri)

HEADERS += control.h
SOURCES += main.cpp \
           control.cpp
FORMS += control.ui

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = control
CONFIG += qt
QT += core gui network

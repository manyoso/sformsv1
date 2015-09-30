include($$PWD/../sforms.pri)

SOURCES += main.cpp

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = asm-quine
CONFIG += qt
QT += core

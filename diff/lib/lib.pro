include($$PWD/../../sforms.pri)

HEADERS += qtdiff.h
SOURCES += qtdiff.cpp

DESTDIR = $$OUTPUT_DIR/lib

TEMPLATE = lib
TARGET = QtDiff
CONFIG += qt
QT += core

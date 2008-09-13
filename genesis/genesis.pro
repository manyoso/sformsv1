include($$PWD/../sforms.pri)

HEADERS += genesis.h
SOURCES += main.cpp \
           genesis.cpp

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = genesis
CONFIG += qt
QT += core network

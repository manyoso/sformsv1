include($$PWD/../sforms.pri)

HEADERS += reaper.h
SOURCES += main.cpp \
           reaper.cpp

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = reaper
CONFIG += qt
QT += core network

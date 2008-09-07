HEADERS += server.h
SOURCES += main.cpp \
           server.cpp

OBJECTS_DIR = tmp
MOC_DIR = tmp

TEMPLATE = app
TARGET = server
CONFIG += qt debug warn_on console silent

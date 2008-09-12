HEADERS += genesis.h
SOURCES += main.cpp \
           genesis.cpp

OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

TEMPLATE = app
TARGET = genesis
CONFIG += qt
QT += core network

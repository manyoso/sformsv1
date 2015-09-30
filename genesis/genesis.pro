include($$PWD/../sforms.pri)

HEADERS += genesis.h
SOURCES += main.cpp \
           genesis.cpp

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = genesis
CONFIG += qt
QT += core network

assembly.name = Generating assembly
assembly.input =  $$TOPLEVELDIR/genesis/sform.cpp
assembly.output = $$DESTDIR/sform.s
assembly.target = sform.s
assembly.commands = $$QMAKE_CXX $$QMAKE_CXXFLAGS -S -O2 \
                    -D_REENTRANT -Wall -W -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB \
                    -I$$INCLUDEPATH -I$$QMAKE_INCDIR_QT -I$$QMAKE_INCDIR_QT/QtCore \
                    -I$$QMAKE_INCDIR_QT/QtNetwork \
                    -o $$assembly.output $$assembly.input && $$DESTDIR/asm-quine $$assembly.output

assembly.CONFIG = no_link

QMAKE_EXTRA_TARGETS += assembly
PRE_TARGETDEPS += sform.s

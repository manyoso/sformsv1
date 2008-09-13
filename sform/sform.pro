include($$PWD/../sforms.pri)

DESTDIR = $$OUTPUT_DIR/bin

CONFIG += qt
QT += core network

CONFIG += release

ASSEMBLY_FILES = sform.cpp
assembly.name = Generating assembly
assembly.input = ASSEMBLY_FILES
assembly.output = ${QMAKE_FILE_BASE}.s
assembly.target = sform.s
assembly.commands = $$QMAKE_CXX $$QMAKE_CXXFLAGS -S -O2 \
                    -D_REENTRANT -Wall -W -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB \
                    -I$$INCLUDEPATH -I$$QMAKE_INCDIR_QT -I$$QMAKE_INCDIR_QT/QtCore \
                    -I$$QMAKE_INCDIR_QT/QtNetwork \
                    -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} && $$DESTDIR/asm-quine ${QMAKE_FILE_OUT}
QMAKE_EXTRA_COMPILERS += assembly

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB
CFLAGS        = -S -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -S -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I../../../../kde/trunk/qt-4.4.0/mkspecs/linux-g++ -I. -I../../../../kde/build/home/kde/trunk/qt-4.4.0/include/QtCore -I../../../../kde/build/home/kde/trunk/qt-4.4.0/include/QtCore -I../../../../kde/build/home/kde/trunk/qt-4.4.0/include/QtNetwork -I../../../../kde/build/home/kde/trunk/qt-4.4.0/include/QtNetwork -I../../../../kde/build/home/kde/trunk/qt-4.4.0/include/QtGui -I../../../../kde/build/home/kde/trunk/qt-4.4.0/include/QtGui -I../../../../kde/build/home/kde/trunk/qt-4.4.0/include -Itmp -Itmp
LINK          = g++
LFLAGS        = -Wl,-rpath,/home/kde/build/home/kde/trunk/qt-4.4.0/lib
LIBS          = $(SUBLIBS)  -L/home/kde/build/home/kde/trunk/qt-4.4.0/lib -lQtGui -lQtNetwork -lQtCore -lpthread

####### Files

SOURCES       = sform.cpp
ASSEMBLY       = tmp/sform.s

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: clean tmp quine tmp/sform.o
	$(LINK) $(LFLAGS) $(LIBS) -o tmp/sform tmp/sform.o

clean:
	rm -rf tmp

tmp:
	mkdir tmp

####### Generate assembly

quine: tmp
	$(CXX) $(CXXFLAGS) $(INCPATH) -o tmp/sform.s sform.cpp
	../asm-quine/asm-quine tmp/sform.s

####### Generate object file

tmp/sform.o:
	as --traditional-format -o tmp/sform.o tmp/sform.s

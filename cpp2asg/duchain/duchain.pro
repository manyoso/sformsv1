HEADERS += util/cursor.h \
           util/cursorfeedback.h \
           util/range.h \
           util/rangefeedback.h \
           util/attribute.h \
           editor/kdevdocumentcursor.h \
           editor/kdevdocumentcursorobject.h \
           editor/kdevdocumentrange.h \
           editor/kdevdocumentrangeobject.h \
           editor/kdeveditorintegrator.h \
           editor/kdeveditorintegrator_p.h \
           duchain.h \
           contextbuilder.h \
           declarationbuilder.h \
           usebuilder.h \
           ducontext.h \
           topducontext.h \
           declaration.h \
           definition.h \
           classmemberdeclaration.h \
           classfunctiondeclaration.h \
           use.h \
           dumpchain.h \
           typesystem.h \
           typebuilder.h \
           cpptypes.h \
           dumptypes.h \
           typerepository.h \
           symboltable.h \
           forwarddeclaration.h \
           duchainbase.h \
           cppeditorintegrator.h \
           ksharedptr.h

SOURCES += util/cursor.cpp \
           util/cursorfeedback.cpp \
           util/range.cpp \
           util/rangefeedback.cpp \
           util/attribute.cpp \
           editor/kdevdocumentcursor.cpp \
           editor/kdevdocumentcursorobject.cpp \
           editor/kdevdocumentrange.cpp \
           editor/kdevdocumentrangeobject.cpp \
           editor/kdeveditorintegrator.cpp \
           duchain.cpp \
           contextbuilder.cpp \
           declarationbuilder.cpp \
           usebuilder.cpp \
           ducontext.cpp \
           topducontext.cpp \
           declaration.cpp \
           definition.cpp \
           classmemberdeclaration.cpp \
           classfunctiondeclaration.cpp \
           use.cpp \
           dumpchain.cpp \
           typesystem.cpp \
           typebuilder.cpp \
           cpptypes.cpp \
           dumptypes.cpp \
           typerepository.cpp \
           symboltable.cpp \
           forwarddeclaration.cpp \
           duchainbase.cpp \
           cppeditorintegrator.cpp

OBJECTS_DIR = tmp
MOC_DIR = tmp

INCLUDEPATH += ../parser util editor
TEMPLATE = lib
TARGET = duchain
VERSION = 0.1
CONFIG += qt debug warn_on console silent

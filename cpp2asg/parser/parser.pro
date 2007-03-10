HEADERS += \
  ast.h \
  control.h \
  identifier.h \
  name_compiler.h \
  rxx_allocator.h \
  visitor.h \
  lexer.h \
  parser.h \
  symbol.h \
  class_compiler.h \
  default_visitor.h \
  listnode.h \
  parsesession.h \
  tokens.h \
  dumptree.h \
  memorypool.h \
  problem.h \
  type_compiler.h \
  rpp/pp-stream.h \
  rpp/pp-macro-expander.h \
  rpp/pp-scanner.h \
  rpp/pp-macro.h \
  rpp/pp-engine.h \
  rpp/pp-internal.h \
  rpp/pp-environment.h \
  rpp/preprocessor.h

SOURCES += \
  ast.cpp \
  default_visitor.cpp \
  memorypool.cpp \
  name_compiler.cpp \
  visitor.cpp \
  class_compiler.cpp \
  control.cpp \
  lexer.cpp \
  parser.cpp \
  listnode.cpp \
  tokens.cpp \
  problem.cpp \
  identifier.cpp \
  dumptree.cpp \
  parsesession.cpp \
  type_compiler.cpp \
  rpp/pp-stream.cpp \
  rpp/pp-macro-expander.cpp \
  rpp/pp-scanner.cpp \
  rpp/pp-macro.cpp \
  rpp/pp-engine.cpp \
  rpp/pp-internal.cpp \
  rpp/pp-environment.cpp \
  rpp/preprocessor.cpp

OBJECTS_DIR = tmp
MOC_DIR = tmp

TEMPLATE = lib
TARGET = parser
VERSION = 0.1
CONFIG += debug warn_on console silent

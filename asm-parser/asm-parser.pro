include($$PWD/../sforms.pri)

SOURCES = afterburner.cpp

DESTDIR = $$OUTPUT_DIR/bin

TEMPLATE = app
TARGET = asm-parser

LIBS    += /usr/lib/libantlr.a

#global defaults
isEmpty(ANTLR_DIR):ANTLR_DIR = .
isEmpty(ANTLR_CMD):ANTLR_CMD = runantlr

ANTLR_GRAMMAR = $$TOPLEVELDIR/asm-parser/Asm.g
ANTLR_GRAMMAR_I = $$ANTLR_DIR/Asm.g.i

######################  PREPROCESS FILES  ######################
antlr_preprocess.commands = cpp -Wall -O2 -I/usr/include -I. -I. -DARCH_ia32 $$ANTLR_GRAMMAR \'-DPREPROCESS_FIRST_MASK(x)=x\' | egrep -v \'^$$LITERAL_HASH \' > $$ANTLR_GRAMMAR_I
antlr_preprocess.CONFIG += no_link
antlr_preprocess.input = ANTLR_GRAMMAR
antlr_preprocess.target = $$ANTLR_GRAMMAR_I
antlr_preprocess.output = $$ANTLR_GRAMMAR_I
antlr_preprocess.clean = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_TARGETS += antlr_preprocess
PRE_TARGETDEPS += $$ANTLR_GRAMMAR_I
QMAKE_CLEAN += $$ANTLR_GRAMMAR_I

######################  CPP FILES  ######################
antlr_lexercpp.commands = $$ANTLR_CMD -o $$ANTLR_DIR ${QMAKE_FILE_NAME}
antlr_lexercpp.variable_out = SOURCES
antlr_lexercpp.output = $$ANTLR_DIR/AsmLexer.cpp
antlr_lexercpp.input = ANTLR_GRAMMAR_I
antlr_lexercpp.clean = $$ANTLR_DIR/AsmLexer.cpp
antlr_lexercpp.depends = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_COMPILERS += antlr_lexercpp

antlr_parsercpp.commands = $$ANTLR_CMD -o $$ANTLR_DIR ${QMAKE_FILE_NAME}
antlr_parsercpp.variable_out = SOURCES
antlr_parsercpp.output = $$ANTLR_DIR/AsmParser.cpp
antlr_parsercpp.input = ANTLR_GRAMMAR_I
antlr_parsercpp.clean = $$ANTLR_DIR/AsmParser.cpp
antlr_parsercpp.depends = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_COMPILERS += antlr_parsercpp

antlr_treecpp.commands = $$ANTLR_CMD -o $$ANTLR_DIR ${QMAKE_FILE_NAME}
antlr_treecpp.variable_out = SOURCES
antlr_treecpp.output = $$ANTLR_DIR/AsmTreeParser.cpp
antlr_treecpp.input = ANTLR_GRAMMAR_I
antlr_treecpp.clean = $$ANTLR_DIR/AsmTreeParser.cpp
antlr_treecpp.depends = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_COMPILERS += antlr_treecpp

######################  HPP FILES  ######################
antlr_lexerhpp.commands = $$ANTLR_CMD -o $$ANTLR_DIR ${QMAKE_FILE_NAME}
antlr_lexerhpp.CONFIG += no_link
antlr_lexerhpp.variable_out = HEADERS
antlr_lexerhpp.output = $$ANTLR_DIR/AsmLexer.hpp
antlr_lexerhpp.input = ANTLR_GRAMMAR_I
antlr_lexerhpp.clean = $$ANTLR_DIR/AsmLexer.hpp
antlr_lexerhpp.depends = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_COMPILERS += antlr_lexerhpp

antlr_parserhpp.commands = $$ANTLR_CMD -o $$ANTLR_DIR ${QMAKE_FILE_NAME}
antlr_parserhpp.CONFIG += no_link
antlr_parserhpp.variable_out = HEADERS
antlr_parserhpp.output = $$ANTLR_DIR/AsmParser.hpp
antlr_parserhpp.input = ANTLR_GRAMMAR_I
antlr_parserhpp.clean = $$ANTLR_DIR/AsmParser.hpp
antlr_parserhpp.depends = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_COMPILERS += antlr_parserhpp

antlr_treehpp.commands = $$ANTLR_CMD -o $$ANTLR_DIR ${QMAKE_FILE_NAME}
antlr_treehpp.CONFIG += no_link
antlr_treehpp.variable_out = HEADERS
antlr_treehpp.output = $$ANTLR_DIR/AsmTreeParser.hpp
antlr_treehpp.input = ANTLR_GRAMMAR_I
antlr_treehpp.clean = $$ANTLR_DIR/AsmTreeParser.hpp
antlr_treehpp.depends = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_COMPILERS += antlr_treehpp

antlr_tokenhpp.commands = $$ANTLR_CMD -o $$ANTLR_DIR ${QMAKE_FILE_NAME}
antlr_tokenhpp.CONFIG += no_link
antlr_tokenhpp.variable_out = HEADERS
antlr_tokenhpp.output = $$ANTLR_DIR/AsmParserTokenTypes.hpp
antlr_tokenhpp.input = ANTLR_GRAMMAR_I
antlr_tokenhpp.clean = $$ANTLR_DIR/AsmParserTokenTypes.hpp
antlr_tokenhpp.depends = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_COMPILERS += antlr_tokenhpp

######################  TXT FILES  ######################
antlr_tokentxt.commands = $$ANTLR_CMD -o $$ANTLR_DIR ${QMAKE_FILE_NAME}
antlr_tokentxt.CONFIG += no_link
antlr_tokentxt.output = $$ANTLR_DIR/AsmParserTokenTypes.txt
antlr_tokentxt.input = ANTLR_GRAMMAR_I
antlr_tokentxt.clean = $$ANTLR_DIR/AsmParserTokenTypes.txt
antlr_tokentxt.depends = $$ANTLR_GRAMMAR_I
QMAKE_EXTRA_COMPILERS += antlr_tokentxt

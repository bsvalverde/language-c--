FLEX=flex
BISON=bison
CC=clang++
PARAM=-std=c++11 -fcxx-exceptions -Wno-deprecated-register -pthread -ldl -ltinfo
LLVM=`llvm-config --cppflags --ldflags --libs core mcjit native`
# LLVMLIB=`llvm-config --ldflags --libs core native` 
# LLVMPARAM=`llvm-config --cppflags`
OUTPUT=c--
TESTFILE=testfile

all: flex yacc clang

clang: *.cpp *.h
	${CC} -o ${OUTPUT} *.cpp ${LLVMPARAM} ${PARAM} ${LLVM}

yacc: parser.y
	${BISON} --defines=parser.h --output=parser.cpp parser.y

flex: lex.l
	${FLEX} -o lex.cpp lex.l 

clean:
	rm parser.cpp parser.h lex.cpp ${OUTPUT}

test: all
	./${OUTPUT} < ${TESTFILE}
CC = g++

CXXFLAGS = -g -O2
LXXDFLAGS =

EXEC = assembler

assembler : assembler.o parser.o compiler.o
	${CC} $^ -o $@ && rm $^
	@clear
	@echo "The file generated is : \"${EXEC}\""

assembler.o : parser.h compiler.h

parser.o : compiler.h

%.o : %.cpp
	${CC} -c $< -o $@ $(CXXFLAGS)

clean :
	rm -f *.o ${EXEC}

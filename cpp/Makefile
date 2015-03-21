CXXFLAGS = -g -O2

EXE = assembler

assembler : assembler.o
	g++ $^ -o $@ && rm $^

$%.o : %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

clean :
	rm *.o

CXXFLAGS = -g -O2

EXE = assembler

assembler : assembler.o
	g++ $^ -o $@ && rm $^
	@clear
	@echo "The file generated is : \"${EXE}\""

$%.o : %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

clean :
	rm *.o

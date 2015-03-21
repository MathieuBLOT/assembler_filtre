#ifndef ASSEMBLER_H
#define ASSEMBLER_H

// #include <iostream>
// #include <fstream>
// #include <string>
// #include <sstream>
// #include <map>
// #include <deque>
// #include <algorithm>
// #include <bitset>
#include "compiler.h"
#include "parser.h"

#define ERROR_OPEN_INSTRUCTION 1
#define ERROR_OPEN_INPUT 2
#define ERROR_OPEN_OUTPUT 3
#define ERROR_UNKNOWN_INSTRUCTION 4
#define ERROR_WRONG_OPERANDS 5

#define STOP_AT_FIRST_ERROR 0
#define PRINT_INSTRUCTION_SET 0

#define OUTPUT_FILE "prog.vhd"

void usage(void);

#endif

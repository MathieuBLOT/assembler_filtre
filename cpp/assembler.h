#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "compiler.h"
#include "parser.h"

#define ERROR_INVALID_ARGUMENTS 1
#define ERROR_OPEN_INSTRUCTION 2
#define ERROR_OPEN_INPUT 3
#define ERROR_OPEN_OUTPUT 4
#define ERROR_UNKNOWN_INSTRUCTION 5
#define ERROR_WRONG_OPERANDS 6
#define ERROR_UNKNOWN_LABEL 7

#define STOP_AT_FIRST_ERROR 0
#define PRINT_INSTRUCTION_SET 0

#define OUTPUT_FILE "prog.vhd"

void usage(void);

#endif

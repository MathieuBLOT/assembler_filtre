#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <deque>
#include <algorithm>
#include <bitset>

#define INSTRUCTION_SET "./instruction_set_binary.txt"
#define MAX_NB_OPERANDS 3
#define NO_REG "000"
#define FULL_REG "111"

typedef std::pair<std::string, int> pair_inst;

void split_string_instruction_set(std::string &toSplit, std::string &instruction,
								  std::string &binary_code, int &number_of_operands);

void print_instruction_set(std::map<std::string, pair_inst> *instruction_set);

void generate_vhdl(std::ofstream &output_file, std::string const &instruction,
				   std::deque<std::string> &operands, int instruction_number,
				   std::string const &original);

void instructionToBinary(std::map<std::string, pair_inst> *instruction_set,
						 std::string &instruction, std::deque<std::string> &operands);

void insert_implicit_operands(std::string &instruction,
							  std::map<std::string, pair_inst>*instruction_set,
							  std::deque<std::string> &operands);

void operandToBinary(std::string &operand);

void add_comment(std::ofstream &output_file, std::string &comment);

#endif

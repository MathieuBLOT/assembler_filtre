#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <map>
#include <deque>
// #include <algorithm>
// #include <bitset>
#include "compiler.h"



std::string trim(const std::string &str, const std::string &whitespace = " \t");

std::string trim_comment(std::string &str, const std::string &commentToken = ";#");

int split_string_instruction(std::string &toSplit, std::string &instruction,
							 std::map<std::string, pair_inst> *instruction_set,
							 std::deque<std::string> *operands);

std::deque<std::string> get_operands(std::string &operandsInString,
									 int number_of_operands);

#endif

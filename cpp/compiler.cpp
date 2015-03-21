#include "compiler.h"

using namespace std;

void split_string_instruction_set(string &toSplit, string &instruction,
								  string &binary_code, int &number_of_operands)
{
	if (!toSplit.empty()) {
		int first_space_pos = toSplit.find_first_of(" ");
		instruction.assign(toSplit.substr(0, first_space_pos));
		binary_code.assign(toSplit.substr(first_space_pos + 1, 7));

		toSplit.erase(0, first_space_pos + 8);
		stringstream int_string(toSplit);
		int_string >> number_of_operands;
	}
}

void print_instruction_set(map<string, pair_inst> *instruction_set){
	for (map<string, pair_inst>::iterator it = instruction_set->begin(); it != instruction_set->end(); ++it) {
		if (it->first == "BRIUMP") {
			cout << "Instruction : [ " << it->first << " ]|\t";
		} else {
			cout << "Instruction : [ " << it->first << " ]\t|\t";
		}
		cout << "Code : [ " << it->second.first << " ]\t|\t";
		if (it->second.second > 0) {
			cout << it->second.second << " operands" << endl;
		} else {
			cout << endl;
		}
	}
}

void generate_vhdl(ofstream &output_file, string const &instruction, deque<string>
&operands, int instruction_number, string const &original)
{
	output_file << instruction_number << " => \"";
	output_file << instruction << "\" & \"";
	while (!operands.empty()) {
		output_file << operands.front();
		operands.pop_front();
		if (!operands.empty())
			output_file << "\" & \"";
	}
	output_file << "\",";
	output_file << " -- " << original;
}

void instructionToBinary(map<string, pair_inst> *instruction_set, string &instruction,
						 deque<string> &operands)
{
	string tmpInstruction;
	transform(instruction.begin(), instruction.end(),instruction.begin(), ::toupper);

	map<string, pair_inst>::iterator it;
	it = instruction_set->find(instruction);
	tmpInstruction = it->first;
	instruction = it->second.first;

	if (tmpInstruction == "LI") {
		instruction = instruction.substr(0, 4);
		instruction += operands.front();
		operands.pop_front();
	}
}

void insert_implicit_operands(string &instruction, map<string, pair_inst> *instruction_set, deque<string> &operands){
	transform(instruction.begin(), instruction.end(), instruction.begin(), ::toupper);
	if (instruction == "INC" || instruction == "DEC" || instruction == "MOVA"
		|| instruction == "NEGA" || instruction == "NOT" || instruction == "SHL1"
		|| instruction == "SHL2" || instruction == "SHL3" || instruction == "SHL4"
		|| instruction == "SHL5" || instruction == "SHL6" || instruction == "SHL7"
		|| instruction == "SHL5" || instruction == "SHL6" || instruction == "SHL7"
		|| instruction == "SHL8" || instruction == "SHL9" || instruction == "SHL10"
		|| instruction == "SHL11" || instruction == "SHL12" || instruction == "SHL13"
		|| instruction == "SHL14" || instruction == "SHL15" || instruction == "SHR1"
		|| instruction == "SHR2" || instruction == "SHR3" || instruction == "SHR4"
		|| instruction == "SHR5" || instruction == "SHR6" || instruction == "SHR7"
		|| instruction == "SHR8" || instruction == "SHR9" || instruction == "SHR10"
		|| instruction == "SHR11" || instruction == "SHR12" || instruction == "SHR13"
		|| instruction == "SHR14" || instruction == "SHR15"	) {
		operands.push_back(NO_REG);
		} else if (instruction == "MOVB" || instruction == "NEGB") {
			string tmp = operands.back();
			operands.pop_back();
			operands.push_back(NO_REG);
			operands.push_back(tmp);
		} else if (instruction == "BREQ" || instruction == "BRGE" || instruction == "BRLE"
			|| instruction == "BRUMP" || instruction == "BRNE" || instruction == "BRLT"
			|| instruction == "BRGT" || instruction == "BAEQ" || instruction == "BAGE"
			|| instruction == "BALE" || instruction == "BAUMP" || instruction == "BANE"
			|| instruction == "BALT" || instruction == "BAGT") {
			operands.push_front(NO_REG);
			} else if (instruction == "IN" || instruction == "OUT") {
				operands.push_back(NO_REG);
				operands.push_back(NO_REG);
			} else if (instruction == "RESET") {
				operands.push_back(FULL_REG);
				operands.push_back(FULL_REG);
				operands.push_back(FULL_REG);
			}
}

void operandToBinary(string &operand){
	if (toupper(operand[0]) == 'R' || operand[0] == '$') {
		operand = bitset < 3 >((int)operand[1]).to_string();
	} else {
		int n(0);
		stringstream int_string(operand);
		int_string >> n;
		operand = bitset < 9 >(n).to_string();
	}
}

void add_comment(ofstream &output_file, string &comment){
	if (!comment.empty())
		output_file << "\t" << comment;
	output_file << endl;
}

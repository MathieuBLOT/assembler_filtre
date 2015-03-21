#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <deque>
#include <algorithm>
#include <bitset>

#define MAX_NB_OPERANDS 3

using namespace std;

typedef pair<string, int> pair_inst;

void usage(){
	cout << "To use this program, type : \"./assembler\" <yourFileToCompile>" << endl;
	cout << "The result of the compilation will be : \"prog.vhdl\"" << endl;
}

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

void operandToBinary(string &operand){
	if (toupper(operand[0]) == 'R' || operand[0] == '$') {
		operand = bitset < 3 >((int)operand[1]).to_string();
	} else if (operand == "NULL") {
		operand == "000";
	} else if (operand == "FULL") {
		operand == "111111111";
	} else {
		int n(0);
		stringstream int_string(operand);
		int_string >> n;
		operand = bitset < 9 >(n).to_string();
	}
}

deque<string> get_operands(string &operandsInString, int number_of_operands){
	string *operandsArray = new string[number_of_operands];
	deque<string> operandsQueue;
	int i(0);

	replace(operandsInString.begin(), operandsInString.end(), ',', ' ');

	stringstream operandsStream(operandsInString);
	while (operandsStream.good() && i < number_of_operands) {
		operandsStream >> operandsArray[i];
		operandToBinary(operandsArray[i]);
		i++;
	}
	for (i = 0; i < number_of_operands; i++) {
		operandsQueue.push_back(operandsArray[i]);
	}
	delete operandsArray;
	return operandsQueue;
}

int split_string_instruction(string &toSplit, string &instruction,
							 map<string, pair_inst> *instruction_set,
							 deque<string> *operands)
{
	int first_space_pos = toSplit.find_first_of(" ");
	instruction.assign(toSplit.substr(0, first_space_pos));
	toSplit.erase(0, first_space_pos);

	map<string, pair_inst>::iterator it;
	it = instruction_set->find(instruction);
	instruction = it->first;
	int number_of_operands = it->second.second;

	*operands = get_operands(toSplit, number_of_operands);

	return (number_of_operands - operands->size());
}

void generate_vhdl_code(ofstream &output_file, string const &instruction,
						deque<string> &operands, int instruction_number)
{
	output_file << instruction_number << " => \"";
	output_file << instruction << "\" & \"";
	while (!operands.empty()) {
		output_file << operands.front();
		operands.pop_front();
	}
	output_file << "\"," << endl;
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
			operands.push_back("NULL");
	} else if (instruction == "MOVB" || instruction == "NEGB") {
		string tmp = operands.back();
		operands.pop_back();
		operands.push_back("NULL");
		operands.push_back(tmp);
	} else if (instruction == "BREQ" || instruction == "BRGE" || instruction == "BRLE"
		|| instruction == "BRUMP" || instruction == "BRNE" || instruction == "BRLT"
		|| instruction == "BRGT" || instruction == "BAEQ" || instruction == "BAGE"
		|| instruction == "BALE" || instruction == "BAUMP" || instruction == "BANE"
		|| instruction == "BALT" || instruction == "BAGT") {
			operands.push_front("NULL");
	} else if (instruction == "IN" || instruction == "OUT") {
			operands.push_back("NULL");
			operands.push_back("NULL");
	} else if (instruction == "RESET") {
			operands.push_back("FULL");
	}
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		usage();
		return 1;
	}

	string line;
	int number_of_operands(0);
	int i(0);
	ifstream instructions_stream("./instruction_set_binary.txt", ios::in);
	ifstream input_file;
	ofstream output_file;
	map<string, pair_inst> instruction_set;

	if (instructions_stream) {
		while ( getline(instructions_stream, line) ) {
			string instruction, code;
			if (!line.empty()) {
				split_string_instruction_set(line, instruction, code, number_of_operands);
				instruction_set[instruction] = make_pair(code, number_of_operands);
			}
		}
		instructions_stream.close();
	} else {
		cerr << "[ ERROR ] File \"instruction_set_binary.txt\"";
		cerr << " could not be opened" << endl;
	}

	print_instruction_set(&instruction_set);

	input_file.open(argv[1], ios::in);
	if (input_file) {
		output_file.open("prog.vhdl", ios::out);
		if (output_file) {
			while ( getline(input_file, line) ) {
				string instruction;
				deque<string> operands;
				int error(0);
				int instruction_number(0);

				i++;
				if (!line.empty()) {
					error = split_string_instruction(line, instruction,
													&instruction_set, &operands);
				}

				if (error) {
					cerr << "[ ERROR ] There is a mistake in";
					cerr << " the number of operands at line" << i << endl;
				}

				insert_implicit_operands(instruction, &instruction_set, operands);

				generate_vhdl_code(output_file, instruction, operands,
								   instruction_number);
				instruction_number++;
			}
			input_file.close();
			output_file.close();
		} else {
			cerr << "[ ERROR ] File \"prog.vhdl\"";
			cerr << " could not be opened" << endl;
		}
	} else {
		cerr << "[ ERROR ] File " << argv[1] << " could not be opened" << endl;
	}

	return 0;
}

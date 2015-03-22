#include "assembler.h"

using namespace std;

void usage(){
	cout << "To use this program, type : \"";
	cout << "./assembler";
	cout << "\" <yourFileToCompile>" << endl;
	cout << "The result of the compilation will be : \"";
	cout << OUTPUT_FILE;
	cout << "\"" << endl;
}



int main(int argc, char* argv[]){
	if (argc != 2) {
		usage();
		return ERROR_INVALID_ARGUMENTS;
	}

	string line;
	int number_of_operands(0);
	int i(0);
	ifstream instructions_stream(INSTRUCTION_SET, ios::in);
	ifstream input_file;
	ofstream output_file;
	map<string, pair_inst> instruction_set;

	if (instructions_stream) {
		while ( getline(instructions_stream, line) ) {
			string instruction, code;
			if (!line.empty()) {
				split_string_instruction_set(line, instruction, code,
											 number_of_operands);
				instruction_set[instruction] = make_pair(code, number_of_operands);
			}
		}
		instructions_stream.close();
	} else {
		cerr << "[ ERROR ] File \"" << INSTRUCTION_SET;
		cerr << "\" could not be opened" << endl;
		return ERROR_OPEN_INSTRUCTION;
	}

#if PRINT_INSTRUCTION_SET != 0
	print_instruction_set(&instruction_set);
#endif

	input_file.open(argv[1], ios::in);
	if (input_file) {
		output_file.open(OUTPUT_FILE, ios::out);
		if (output_file) {
			int instruction_number(0);
			while ( getline(input_file, line) ) {
				string original_instruction;
				string instruction;
				string comment;
				deque<string> operands;
				int error(0);

				i++;
				if (!line.empty()) {
					line = trim(line);
					comment = trim_comment(line);
					original_instruction = line;
					if (!line.empty()) {
						error = split_string_instruction(line, instruction,
														&instruction_set, &operands);

						if (error) {
							output_file << "-- Line skipped due to error";
							output_file << " (unrecognized instruction)";
							if (error == 127) {
								cerr << "Unknown instruction at line : ";
								cerr << i << endl;
#if STOP_AT_FIRST_ERROR != 0
								return ERROR_UNKNOWN_INSTRUCTION;
#endif
							} else {
								cerr << "[ ERROR ] There is a mistake in";
								cerr << " the number of operands at line : ";
								cerr << i << endl;
#if STOP_AT_FIRST_ERROR != 0
								return ERROR_WRONG_OPERANDS;
#endif
							}
						} else {

							insert_implicit_operands(instruction, &instruction_set,
													operands);

							instructionToBinary(&instruction_set, instruction,
												operands);
							generate_vhdl(output_file, instruction, operands,
										instruction_number, original_instruction);
						}
						instruction_number++;
					}
					add_comment(output_file, comment);
				}
			}
			output_file.close();
		} else {
			cerr << "[ ERROR ] File \"" << OUTPUT_FILE;
			cerr << "\" could not be opened" << endl;
			return ERROR_OPEN_OUTPUT;
		}
		input_file.close();
	} else {
		cerr << "[ ERROR ] File " << argv[1] << " could not be opened" << endl;
		return ERROR_OPEN_INPUT;
	}

	return EXIT_SUCCESS;
}

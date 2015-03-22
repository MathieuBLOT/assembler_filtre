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
			map<string, int> labels;

			while ( getline(input_file, line) ) {
				if (!line.empty()) {
					line = trim(line);
					trim_comment(line);
					if (!line.empty()) {

						if (line[line.length() - 1] == ':') {
							labels[line.substr(0, line.length() -1)] = instruction_number;
						} else {
							instruction_number++;
						}
					}
				}
			}

			instruction_number = 0;
			input_file.clear();
			input_file.seekg(0, ios::beg);

			while ( getline(input_file, line) ) {
				string original_instruction;
				string instruction;
				string comment;
				deque<string> operands;
				int offset(0);
				int error(0);

				i++;
				if (!line.empty()) {
					line = trim(line);
					comment = trim_comment(line);
					original_instruction = line;
					if (line[line.length() - 1] == ':') {
						comment = "-- " + line + comment;
						line = "";
					}
					if (!line.empty()) {
						string branch = line.substr(0, 3);
						transform(branch.begin(), branch.end(), branch.begin(),
								  ::toupper);
						if (branch == "BRI") {
							int space_pos = line.find_first_of(" ");
							string immediate = line.substr(space_pos + 1,
														   line.length() - space_pos);
							map<string, int>::iterator it;

							it = labels.find(immediate);
							if (it == labels.end()) {
								cerr << "[ ERROR ] Unknown label at line : ";
								cerr << i << endl;
								#if STOP_AT_FIRST_ERROR != 0
								return ERROR_UNKNOWN_LABEL;
								#endif
							} else {
								// Replace Immediate with:
								offset = (it->second) - instruction_number;
								ostringstream conversionStream;
								conversionStream << offset;
								line.replace(space_pos + 1, line.length() - space_pos,
											 conversionStream.str());
							}
						}

						error = split_string_instruction(line, instruction,
														&instruction_set, &operands);

						if (error) {
							output_file << "-- Line skipped due to error";
							if (error == 127) {
								output_file << " (unrecognized instruction)";
								cerr << "Unknown instruction at line : ";
								cerr << i << endl;
#if STOP_AT_FIRST_ERROR != 0
								return ERROR_UNKNOWN_INSTRUCTION;
#endif
							} else {
								output_file << " (wrong number of operands)";
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

	cout << "[ SUCCESS ] The resulting file is : \"";
	cout << OUTPUT_FILE;
	cout << "\"" << endl;

	return EXIT_SUCCESS;
}

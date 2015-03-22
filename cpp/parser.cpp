#include "parser.h"

using namespace std;

string trim(const string &str, const string &whitespace){
	const size_t strBegin = str.find_first_not_of(whitespace);
	if (strBegin == string::npos)
		return ""; // no content

		const size_t strEnd = str.find_last_not_of(whitespace);
	const size_t strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

string trim_comment(string &str, const string &commentToken){
	const size_t commentBegin = str.find_first_of(commentToken);
	string comment;
	if (commentBegin == string::npos)
		return ""; // no content

		comment = str.substr(commentBegin, string::npos);
	comment = comment.replace(0, 1, "-- ");
	str = str.substr(0, commentBegin);
	return comment;
}

int split_string_instruction(string &toSplit, string &instruction,
							 map<string, pair_inst> *instruction_set,
							 deque<string> *operands)
{
	int first_space_pos = toSplit.find_first_of(" ");
	instruction.assign(toSplit.substr(0, first_space_pos));
	toSplit.erase(0, first_space_pos);

	transform(instruction.begin(), instruction.end(),instruction.begin(), ::toupper);

	map<string, pair_inst>::iterator it;
	it = instruction_set->find(instruction);
	if (it == instruction_set->end()) {
		return 127;
	}
	int number_of_operands = it->second.second;

	*operands = get_operands(toSplit, number_of_operands);

	return (number_of_operands - operands->size());
}

deque<string> get_operands(string &operandsInString, int number_of_operands){
	string operandsArray[number_of_operands];
	deque<string> operandsQueue;
	int number_of_found_operands(0);
	int i(0);

	replace(operandsInString.begin(), operandsInString.end(), ',', ' ');

	stringstream operandsStream(operandsInString);
	while (operandsStream.good() && i < number_of_operands) {
		operandsStream >> operandsArray[i];
		operandToBinary(operandsArray[i]);
		i++;
		number_of_found_operands++;
	}
	for (i = 0; i < number_of_found_operands; i++) {
		operandsQueue.push_back(operandsArray[i]);
	}

	return operandsQueue;
}

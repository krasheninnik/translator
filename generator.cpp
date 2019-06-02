#include "pch.h"
#include "generator.h"
#include  <fstream>
#include <string>

Generator::Generator(ConstTable* _keyWords, ConstTable* _signOfOperations, ConstTable* _delimeters,
			ChangedTable* _identificators, ChangedTable* _consts) {
	// link tables:
	keyWords = _keyWords;
	signOfOperations = _signOfOperations;
	delimeters = _delimeters;

	identificators = _identificators;
	consts = _consts;
	   

}


void Generator::generate(std::vector<std::vector<Token>>& postfixEntries) {
	std::ofstream fout("output\\code.txt");

	// generate general construction:
	fout << ".386" << std::endl <<
		".MODEL FLAT, STDCALL" << std::endl <<
		"EXTERN  ExitProcess@4: PROC; функция выхода из программы\n" <<
		".DATA" << std::endl;

#pragma region dataAllocation
	// generate segment of static data:
	// memory allocation for variables:
	auto elems = identificators->getElems();

	for (int i = 0; i < elems.size(); i++) {
		for (int j = 0; j < elems[i].size(); j++) {
			// formating string of code to memory allocation
			auto& elem = elems[i][j];

			switch (elem.type) {
			case TYPE::CHAR: fout << elem.name + " db ?\n"; break; // DB Define Byte, allocates 1 byte
			case TYPE::INT: fout << elem.name + " dd ?\n"; break; // DD Define Doubleword, allocates 4 bytes
			}
		}
	}
#pragma endregion

	// generate code
	fout << ".CODE" << std::endl << "MAIN PROC" << std::endl;

	// processing postfix entries
	for (auto& entry : postfixEntries) {
		fout << processingPostfixEntry(entry);
	}

	fout << "; exit from program\n" <<
		"PUSH 0 ; return code\n" <<
		"CALL ExitProcess@4\n" <<
        "MAIN ENDP\nEND MAIN\n";
}

std::string Generator::getTokenStr(const Token& t) {
	std::string result;

	switch (t.table) {
	case TABLE::KEYWORDS: result = (*keyWords)[t.place]; break;
	case TABLE::OPERATIONS: result = (*signOfOperations)[t.place]; break;
	case TABLE::DELIMETERS: result = (*delimeters)[t.place]; break;
	case TABLE::IDENTIFIATORS: result = identificators->getName(t.place, t.place2);  break;
	case TABLE::CONSTS: result = consts->getName(t.place, t.place2);  break;
	default: throw std::string("Undefined type of table");
	}

	return result;
}

std::string Generator::processingPostfixEntry(std::vector<Token> entry) {                          
	
	// state:
	bool findResultToken = false;
	Token resultToken; // token of result element: result = operand1 operation1 operand2....

	// validate of postfix entry:
	if (entry.back().table == TABLE::DELIMETERS) entry.pop_back();
	else throw std::string("bad postfix entry to generate code");
	
	if (entry.size() == 1) return std::string("");

	// commentary
	std::string cmd = "; start of statement\n";

	// processing entry:
	static unsigned int numberOfcurrentLabel = 0;

	for(int i = 0; i < entry.size(); i++) {
		auto& token = entry[i];

		if (token.table == TABLE::OPERATIONS) {
			auto operation = signOfOperations->getName(token.place); // recognize what operation

			if (operation == "=") {
				TYPE t = identificators->getType(resultToken.place, resultToken.place2);
				std::string name = identificators->getName(resultToken.place, resultToken.place2);


				// pop result of sequence of opearations  from stack
				if (t == TYPE::INT) {
					cmd += "pop " + name + "\n";
				}
				else {
					cmd += "pop edx\n";
					cmd += "mov " + name + ", dl\n";
				}

				cmd += "pop eax\n"; // pop result elems from stack
			}
			else {
				// if shift operation = need ecx register instead eax.
				if (operation == "<<" || operation == ">>") { cmd += "pop ecx\n"; }
				else { cmd += "pop eax\n"; }

				cmd += "pop ebx\n";

				if (operation == "+") cmd += "add ebx, eax\npush ebx\n";
				if (operation == "-") cmd += "sub ebx, eax\npush ebx\n";
				if (operation == "<<") cmd += "sal ebx, cl\npush ebx\n";
				if (operation == ">>") cmd += "sar ebx, cl\npush ebx\n";
				if (operation == "&")  cmd += "and ebx, eax\npush ebx\n";
				if (operation == "|")  cmd += "or ebx, eax\npush ebx\n";


				if (operation == ">" || operation == "<") {
					std::string successLabel = std::string("labelForSuccessJmp_") + std::to_string(numberOfcurrentLabel);
					std::string resultLabel = std::string("labelForResultJmp_") + std::to_string(numberOfcurrentLabel);

					cmd += "cmp ebx, eax\n";

					if (operation == ">") cmd += "jg " + successLabel + "\n"; // >
					else cmd += "jl " + successLabel + "\n"; // <

					cmd += "push 0\n";
					cmd += "jmp " + resultLabel + "\n";
					cmd += successLabel + ":\n";
					cmd += "push 1\n";
					cmd += resultLabel + ":\n";
					numberOfcurrentLabel++;
				}

			}

		}
		else {
			
			if (token.table == TABLE::IDENTIFIATORS) {		

				const TYPE t = identificators->getType(token.place, token.place2);
				if (t == TYPE::INT) {
					cmd += "push " + getTokenStr(token) + "\n";
				}
				else { // t == TYPE::CHAR
					// zeroing register
					cmd += "xor edx, edx\n";
					cmd += "mov dl, " + getTokenStr(token) + "\n";
					cmd += "push edx\n";
				}

				if (findResultToken == false) {
					resultToken = token;
					findResultToken = true;
				}
			}

			else cmd += "push " + consts->getName(token.place, token.place2) + "\n";
		}
	}

	// commentary
	cmd += "; end of statemnt\n";
	return cmd;
}
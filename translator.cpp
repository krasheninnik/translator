#include "pch.h"
#include "translator.h"
#include <iostream>
#include <fstream>
#include <string>

Translator::Translator() {
	// initiation const tables:
	std::vector<std::string> keyWordsInitList{"char","int",	};
	std::vector<std::string> signOfOperationsInitList{ "=", "+","-","<",">","<<",">>", "&","|" };
	std::vector<std::string> delimetersInitList{ ",","\n","\t"," ",";" };

	keyWords =	ConstTable(TABLE::KEYWORDS, keyWordsInitList);
	signOfOperations = ConstTable(TABLE::OPERATIONS, signOfOperationsInitList);
	delimeters = ConstTable(TABLE::DELIMETERS, delimetersInitList);
	
	// initiation changed tables:
	const int identTableSize = 1000;
	const int constTableSize = 1000;

	identificators = ChangedTable(TABLE::IDENTIFIATORS, identTableSize);
	consts = ChangedTable(TABLE::CONSTS, constTableSize);

	// initiation lexical analizator:
	lexAnaliz = LexicalAnaliz();
	tokens = std::vector<Token>();
}

Token Translator::lexemeToToken(STATE state, std::string lexeme) {
	Token result;

	switch (state) {
	
	case STATE::IDENTIFICATOR: // key words + identifecators
		result = keyWords.find(lexeme);
		if (result.place == -1) result = identificators.findAdd(lexeme);
		break;

	case STATE::OPERTAION:	  // operation
	case STATE::OPERATION2RIGHT:
	case STATE::OPERATION2RIGHT2:
	case STATE::OPERATION2LEFT:
	case STATE::OPERATION2LEFT2:
		result = signOfOperations.find(lexeme);		
		break;

	case STATE::DELIMETER:	// delimeters
		result = delimeters.find(lexeme);
		break;

	case STATE::INT:		 // consts:
		result = consts.findAdd(lexeme);
		break;

	default:
		throw std::string("Don't need forming token");
	}

	return result;
}

inline bool Translator::needToToken(STATE state) {
	return state == STATE::IDENTIFICATOR ||
		state == STATE::INT ||
		state == STATE::DELIMETER ||
		state == STATE::OPERTAION ||
		state == STATE::OPERATION2RIGHT ||
		state == STATE::OPERATION2RIGHT2 ||
		state == STATE::OPERATION2LEFT ||
		state == STATE::OPERATION2LEFT2;
}

void Translator::translateToTokens() {
	std::fstream fin(R"(input/input.txt)");
	std::ofstream ferr(R"(output/lexical/error_log.txt)");
	std::string lexeme;

	bool succesful = true;
	int errorCount = 0;

	int lineNum = 0;
	int symbol = 0;
	int row = 0;

	STATE curState = START;
	STATE nextState = START;

	// need add \n to line or special processing \n;
	while (fin) {
		std::string line;
		std::getline(fin, line); line += '\n';

		EVENT event;

		std::string lexeme = "";

		for (int i = 0; i < line.size(); i++) {	// loop through line
			event = lexAnaliz.recognizeEvent(line[i]);
			nextState = lexAnaliz.transition(curState, event);


			/// add char to const!


			// processing state:
			if (nextState == STATE::OUT) {
				if (curState == STATE::ERROR) {
					lexAnaliz.errorLog(ferr, row, lexeme);
					succesful = false;
					errorCount++;
				}
				else{
					if (event != DELIMETER_SKIP_EV && event != NEW_LINE) i--;

					// processing lexeme: token <-- lexeme
					if (needToToken(curState)) {
						Token token = lexemeToToken(curState, lexeme);
						tokens.push_back(token);
						// to debug:
						std::cout << lexeme << std::endl;
					}
				}

				// prepare to recognize next lexeme:
				lexeme = "";
				nextState = START;
			}
			else lexeme += line[i];

			// prepare next iteration:
			std::swap(curState, nextState);
		}
	}
	
	if (!fin && curState == COMMENT) {
		succesful = false;
		std::cout << "Unclosed comment";
	}

	if (succesful) std::cout << "Lexical analiz has been successfully completed\n";
	else std::cout << "Lexical analiz has been completed with " << errorCount << " error(s)\n";

	logTables();
	logTokens();
}

#pragma region logs

void Translator::logTables() {
	std::ofstream fout(R"(output/lexical/tables.txt)");
	fout << "Table of Identificators" << std::endl;
	identificators.logTable(fout);
	fout << std::endl << "Table of Const" << std::endl;
	consts.logTable(fout);
}

void Translator::logTokens() {
	std::ofstream fout(R"(output/lexical/tokens.txt)");

	for (int i = 0; i < tokens.size(); i++) {
			fout << tokens[i].table << " " <<
				tokens[i].place << " " <<
				tokens[i].place2 << ", ";
	}
}

#pragma endregion
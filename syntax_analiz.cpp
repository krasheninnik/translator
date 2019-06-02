#include "pch.h"
#include "translator.h"
#include "syntax_analiz.h"
#include "lexical_analiz.h"

#include <functional>
#include <iostream>

//using func = std::function<void(const Translator&)>;

SyntaxAnaliz::SyntaxAnaliz(ConstTable* _keyWords , ConstTable* _signOfOperations, ConstTable* _delimeters,
	ChangedTable* _identificators, ChangedTable* _consts) {

	// link tables:
	keyWords = _keyWords;
	signOfOperations = _signOfOperations;
	delimeters = _delimeters;

	identificators = _identificators;
	consts = _consts;

	// init syntax analizator
	indexOfNextToken = 0;
	stack = std::stack<int>();


	// init parser table
	table = std::vector<ParseElem>(68);

#pragma region setPriority
	priority["="] = 1;
	priority[";"] = 1;

	priority["|"] = 2;
	priority["&"] = 3;

	priority["<"] = 4;
	priority[">"] = 4;

	priority["<<"] = 5;
	priority[">>"] = 5;

	priority["+"] = 6;
	priority["-"] = 6;
#pragma endregion

	// fill parser table
#pragma region fillParserTable
	table[0] = { 1, std::vector<std::string>{"int", "char", "varId"},false, false, false, true };
	table[1] = { 2, std::vector<std::string>{"int", "char", "varId"},false, false, false, true };
	table[2] = { 5, std::vector<std::string>{"int", "char"},false, false, false, false };
	table[3] = { 7, std::vector<std::string>{"varId"},false, false, false, false };
	table[4] = { 9, std::vector<std::string>{""},false, false, false, true };
	table[5] = { 10, std::vector<std::string>{"int", "char"},false, true, false, true };
	table[6] = { 1, std::vector<std::string>{"int", "char", "varId"},false, false, false, true };
	table[7] = { 57, std::vector<std::string>{"varId"},false, true, false, true };
	table[8] = { 1, std::vector<std::string>{"int", "char", "varId"},false, false, false, true };
	table[9] = { -1, std::vector<std::string>{""},true, false, true, true };
	table[10] = { 11, std::vector<std::string>{"int", "char"},false, false, false, true };
	table[11] = { 52, std::vector<std::string>{"int", "char"},false, true, false, true };
	table[12] = { 14, std::vector<std::string>{"varId"},false, true, false, true };
	table[13] = { -1, std::vector<std::string>{";"},true, false, true, true };
	table[14] = { 15, std::vector<std::string>{"varId"},false, false, false, true };
	table[15] = { 16, std::vector<std::string>{"varId"},true, false, false, true };
	table[16] = { 23, std::vector<std::string>{",", "=", ";"},false, true, false, true };
	table[17] = { 18, std::vector<std::string>{",", ";"},false, false, false, true };
	table[18] = { 20, std::vector<std::string>{","},false, false, false, false };
	table[19] = { 22, std::vector<std::string>{";"},false, false, false, true };
	table[20] = { 21, std::vector<std::string>{","},true, false, false, true };
	table[21] = { 14, std::vector<std::string>{"varId"},false, false, false, true };
	table[22] = { -1, std::vector<std::string>{";"},false, false, true, true };
	table[23] = { 25, std::vector<std::string>{"="},false, false, false, false };
	table[24] = { 27, std::vector<std::string>{",", ";"},false, false, false, true };
	table[25] = { 26, std::vector<std::string>{"="},true, false, false, true };
	table[26] = { 28, std::vector<std::string>{"varId"},false, false, false, true };
	table[27] = { -1, std::vector<std::string>{",", ";"},false, false, true, true };
	table[28] = { 29, std::vector<std::string>{"varId"},false, false, false, true };
	table[29] = { 30, std::vector<std::string>{"varId"},true, false, false, true };
	table[30] = { 31, std::vector<std::string>{";", ",", "+", "-", ">", "<", ">>", "<<", "&", "|"},false, false, false, true };
	table[31] = { 33, std::vector<std::string>{"+", "-", ">", "<", ">>", "<<", "&", "|", ""},false, false, false, false };
	table[32] = { 35, std::vector<std::string>{";", ","},false, false, false, true };
	table[33] = { 36, std::vector<std::string>{"+", "-", ">", "<", ">>", "<<", "&", "|", ""},false, true, false, true };
	table[34] = { 28, std::vector<std::string>{"varId"},false, false, false, true };
	table[35] = { -1, std::vector<std::string>{";", ","},false, false, true, true };
	table[36] = { 44, std::vector<std::string>{"+"},false, false, false, false };
	table[37] = { 45, std::vector<std::string>{"-"},false, false, false, false };
	table[38] = { 46, std::vector<std::string>{">"},false, false, false, false };
	table[39] = { 47, std::vector<std::string>{"<"},false, false, false, false };
	table[40] = { 48, std::vector<std::string>{">>"},false, false, false, false };
	table[41] = { 49, std::vector<std::string>{"<<"},false, false, false, false };
	table[42] = { 50, std::vector<std::string>{"&"},false, false, false, false };
	table[43] = { 51, std::vector<std::string>{"|"},false, false, false, true };
	table[44] = { -1, std::vector<std::string>{"+"},true, false, true, true };
	table[45] = { -1, std::vector<std::string>{"-"},true, false, true, true };
	table[46] = { -1, std::vector<std::string>{">"},true, false, true, true };
	table[47] = { -1, std::vector<std::string>{"<"},true, false, true, true };
	table[48] = { -1, std::vector<std::string>{">>"},true, false, true, true };
	table[49] = { -1, std::vector<std::string>{"<<"},true, false, true, true };
	table[50] = { -1, std::vector<std::string>{"&"},true, false, true, true };
	table[51] = { -1, std::vector<std::string>{"|"},true, false, true, true };
	table[52] = { 53, std::vector<std::string>{"int", "char"},false, false, false, true };
	table[53] = { 55, std::vector<std::string>{"int"},false, false, false, false };
	table[54] = { 56, std::vector<std::string>{"char"},false, false, false, true };
	table[55] = { -1, std::vector<std::string>{"int"},true, false, true, true };
	table[56] = { -1, std::vector<std::string>{"char"},true, false, true, true };
	table[57] = { 58, std::vector<std::string>{"varId"},false, false, false, true };
	table[58] = { 59, std::vector<std::string>{"varId"},true, false, false, true };
	table[59] = { 62, std::vector<std::string>{"=", "+=", "-="},false, true, false, true };
	table[60] = { 28, std::vector<std::string>{"varId"},false, true, false, true };
	table[61] = { -1, std::vector<std::string>{";"},true, false, true, true };
	table[62] = { 65, std::vector<std::string>{"="},false, false, false, true };
	table[63] = { 66, std::vector<std::string>{"+="},false, false, false, true };
	table[64] = { 67, std::vector<std::string>{"-="},false, false, false, true };
	table[65] = { -1, std::vector<std::string>{"="},true, false, true, true };
	table[66] = { -1, std::vector<std::string>{"+="},true, false, true, true };
	table[67] = { -1, std::vector<std::string>{"-="},true, false, true, true };
#pragma endregion


}

Token SyntaxAnaliz::getNextToken(std::vector<Token>& tokens) {
	return tokens[indexOfNextToken++];
}

bool SyntaxAnaliz::endOfTokens(std::vector<Token>& tokens) {
	return indexOfNextToken == tokens.size();
}

bool SyntaxAnaliz::endOfParse(Token& token) {
	static const auto endToken = signOfOperations->find("");
	return token.table == endToken.table & token.place == endToken.place;
}

bool SyntaxAnaliz::isPossibleTerminal(const std::string& theTerm) {
	auto& possibleTerms = table[currRow].terminals;
	auto it = std::find(possibleTerms.begin(), possibleTerms.end(), theTerm);
	return !(it == possibleTerms.end());
	// true, if theTerm was found
	// false, in another way
}

std::string SyntaxAnaliz::getTokenStr(const Token& t) {
	std::string result;

	switch(t.table) {
	case TABLE::KEYWORDS: result = (*keyWords)[t.place]; break;
	case TABLE::OPERATIONS: result = (*signOfOperations)[t.place]; break;
	case TABLE::DELIMETERS: result = (*delimeters)[t.place]; break;
	case TABLE::IDENTIFIATORS: result = identificators->getName(t.place, t.place2);  break;
	case TABLE::CONSTS: result = consts->getName(t.place, t.place2);  break;
	default: throw std::string("Undefined type of table");
	}

	return result;
}

void SyntaxAnaliz::parse(std::vector<Token>& tokens) {
	bool localError = false;		// error detectiong
	Token currToken, nextToken; // current and next Tokens
	
	currToken = getNextToken(tokens);
	nextToken = getNextToken(tokens);

	currRow = 0; // current row in parse table
	prevRow = 0; // previous row in parse table

	bool haveType = false; // detecting type in this moment
	TYPE type;			   // detected type

	// variables for bulding parse tree
	bool littleTreeBG = false; // building of little tree in this moment
	std::string tokenStrPrev; // terminal of previous token
	int idNumber;				// number of Identificator

	std::vector<Token> littleTreeCode; // tokens for create little tree

	// parse:
	bool continueParse = true;
	while (continueParse && !localError) {
			
		if (endOfParse(currToken)) break;

		std::string tokenStr = getTokenStr(currToken); // terminal of tokens
		std::string nextTokenStr = "";

		//if (!endOfTokens(tokens)) {
			nextTokenStr = getTokenStr(nextToken);
		//}
		
		if(!continueParse) tokenStr = "";


		if (currToken.table == TABLE::IDENTIFIATORS || currToken.table == TABLE::CONSTS) {
			tokenStr = "varId";
		}

		if (tokenStr == "varId") littleTreeBG = true;

		if (isPossibleTerminal(tokenStr)) {
			bool changeRow = false;			 // response for the change of row in parse table;

			if (table[currRow].stack) 
				stack.push(currRow + 1);	// add Row in stack

			if (table[currRow].accept) {	// do we accept this terminal?

				if (littleTreeBG) {	// expand the tree
					littleTreeCode.push_back(currToken);
				}

				if (tokenStr == ";" || tokenStr == ",") { // end of parsing single operator
					addPostfixEntry(littleTreeCode); // add code in tree;

					// reset state:
					littleTreeBG = false;
					littleTreeCode.clear();
				}

				// invalidate current state of haveType
				if (tokenStr == ";") haveType = false;

				// if founded type = remember them
				if (tokenStr == "int" || tokenStr == "char") {
					haveType = true;

					if (tokenStr == "int") type = TYPE::INT;
					if (tokenStr == "char") type = TYPE::CHAR;
				}

//////////////////////////////////////////////////////////////////////////////////////
				// Add type in table of identificators:
#pragma region TypeDetection
				static const int TYPE_ASSIGN_POINT = 15; // from table;
				if (tokenStr == "varId" && currRow == TYPE_ASSIGN_POINT) {
					if (currToken.table == TABLE::IDENTIFIATORS) {
						if (identificators->getType(currToken.place, currToken.place2) == TYPE::UNDEFINED) {
							identificators->setType(currToken.place, currToken.place2, type);
						}
						else {
							localError = true;
							std::cout << "SA Error. Type overriding." << std::endl;
							break;
						}
					}
					else {
						localError = true;
						std::cout << "SA Error. Expected Identificator" << std::endl;
						break;
					}
				}

				static const int ASSIGN_POINT = 58; // from table;
				if (tokenStr == "varId" && currRow == ASSIGN_POINT) {
					if (currToken.table == TABLE::IDENTIFIATORS) {
						if (identificators->getType(currToken.place, currToken.place2) == TYPE::UNDEFINED) {
							localError = true;
							std::cout << "SA Error. Variable not defined." << std::endl;
							break;
						}
					}
					else {
						localError = true;
						std::cout << "SA Error. Expected L-value." << std::endl;
						break;
					}
				}
#pragma endregion

#pragma region InitializeDetection
				// set init = true
				static const int INIT_POINT_1 = 15; // from table;

				if (tokenStr == "varId" && currRow == INIT_POINT_1 && nextTokenStr == "=") {
					identificators->setInit(currToken.place, currToken.place2, true);
				}


				static const int INIT_POINT_2 = 58; // from table;

				if (tokenStr == "varId" && currRow == INIT_POINT_2 && nextTokenStr == "=") {
					identificators->setInit(currToken.place, currToken.place2, true);
				}

				// check initialization
				

				if(currToken.table == TABLE::IDENTIFIATORS) {
					static const int CHECK_INIT_POINT = 29; // from table;
					if (tokenStr == "varId" && currRow == CHECK_INIT_POINT) {
						if (identificators->getInit(currToken.place, currToken.place2) == false) {
							localError = true;
							std::cout << "SA Error. Used not inited variable." << std::endl;
							break;
						}

						identificators->setInit(currToken.place, currToken.place2, true);
					}
				}

#pragma endregion

				// prepare next iteration
				currToken = nextToken;

				if (endOfParse(currToken)) break;
				else nextToken = getNextToken(tokens);
			}

			// processing returns column
			if (table[currRow].returns) {
				prevRow = currRow;
				currRow = stack.top();
				stack.pop();
				changeRow = true;
			}

			// processing jump coulmm
			if (table[currRow].jump != -1 && !changeRow) {
				currRow = table[currRow].jump;
			}


		}
		else { // if terminals not found in this row
			if (table[currRow].error) {
				localError = true;
				std::cout << "Syntax analiz ended with error\n";
			}
			else {
				currRow++;
			}

		}
	}

	if(!localError) std::cout << "Syntax analiz successful end." << std::endl;


	for (auto ent : postfixEntries) {
		for (auto sg : ent) std::cout << getTokenStr(sg).c_str() << " ";
		std::cout << std::endl;
	}

	//return localError;
}

//По результатам разбора для каждого оператора исходной программы можно построить синтаксическое дерево, удовлетворяющее следующим требованиям :
//1. Ключевые слова и знаки операций являются корнями непустых поддеревьев.
//2. Идентификаторы и константы являются листьями.
//Концевой(постфиксный) обход синтаксического дерева позволяет получить постфиксную(обратную польскую) запись.

//  Представление кода smallTreeCode в виде дерева,
//  при конце разбора целостного оператора. (";" or ",");

void SyntaxAnaliz::addPostfixEntry(std::vector<Token>& code) {
	std::vector<Token> postfixEntry = formingPostfixEntry(code);
	postfixEntries.push_back(postfixEntry);
}


// Метод Е. В. Дикстры основан на использовании стека с приоритетами.
std::vector<Token> SyntaxAnaliz::formingPostfixEntry(std::vector<Token>& code) {
	std::vector<Token> result;
	std::stack<Token> s;


	// processinf code string:
	for (int i = 0; i < code.size(); i++) {
		Token token = code[i];

		if (token.table == TABLE::IDENTIFIATORS || token.table == TABLE::CONSTS) {
			result.push_back(token);
		}
		else {
			if (s.empty()) s.push(token);
			else {
				if (priority[getTokenStr(token)] > priority[getTokenStr(s.top())]) {
					s.push(token);
				}
				else {
					do {
						result.push_back(s.top());
						s.pop();
					} while (!s.empty() && priority[getTokenStr(token)] <= priority[getTokenStr(s.top())]);

					s.push(token);
				}
			}
		}
	}

	while (!s.empty()) {
		result.push_back(s.top());
		s.pop();
	}

	return result;
}
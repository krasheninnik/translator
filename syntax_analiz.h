#pragma once
#include <vector>
#include <stack>
#include "header.h"
#include <map>

struct ParseElem {
	int jump;
	std::vector<std::string> terminals;
	bool accept;
	bool stack;
	bool returns;
	bool error;
};

struct TreeElem {
	TreeElem* left;
	TreeElem *right;

	Token info;
};

class SyntaxAnaliz {
public:
	SyntaxAnaliz() = default;
	SyntaxAnaliz(ConstTable*, ConstTable*, ConstTable*, ChangedTable*, ChangedTable*);
	void parse(std::vector<Token>&);

private:
	// links on tables:
	ConstTable* keyWords;
	ConstTable* signOfOperations;
	ConstTable* delimeters;
	
	ChangedTable* identificators;
	ChangedTable* consts;
	//

	Token getNextToken(std::vector<Token>&);
	bool endOfTokens(std::vector<Token>&);
	bool endOfParse(Token&);
	bool isPossibleTerminal(const std::string&);
	std::string getTokenStr(const Token&);


	std::vector<ParseElem> table;
	std::stack<int> stack;

	int currRow;
	int prevRow;

	int indexOfNextToken; // used to extraction next token from array;

	// trees of parse:
	std::map<std::string, int> priority;
	void addPostfixEntry(std::vector<Token>& code);
	std::vector<Token> formingPostfixEntry(std::vector<Token>& code);

public:
	std::vector<std::vector<Token>> postfixEntries;
};
#pragma once
#include "header.h"
#include "table.h"

class Generator {
public:
	Generator() = default;
	Generator(ConstTable*, ConstTable*, ConstTable*, ChangedTable*, ChangedTable*);
	void generate(std::vector<std::vector<Token>>&);

private:
	// links on tables:
	ConstTable* keyWords;
	ConstTable* signOfOperations;
	ConstTable* delimeters;

	ChangedTable* identificators;
	ChangedTable* consts;                                             
	//
	            
	//TYPE figureoutTypeOfOpearation(const Token& o1, const Token& o2);
	std::string getTokenStr(const Token&);
	std::string processingPostfixEntry(std::vector<Token> entry);
};
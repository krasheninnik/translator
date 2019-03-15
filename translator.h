#pragma once
#include "table.h"
#include "lexical_analiz.h"

class Translator {
public:
	Translator();
	void translateToTokens();
	void logTables();
	void logTokens();

private:
	Token lexemeToToken(STATE, std::string);
	bool needToToken(STATE);

private:
	LexicalAnaliz lexAnaliz;

	ConstTable keyWords;
	ConstTable signOfOperations;
	ConstTable delimeters;

	ChangedTable identificators;
	ChangedTable consts;

	std::vector<Token> tokens;
};
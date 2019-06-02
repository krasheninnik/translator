#pragma once
#include "table.h"
#include "lexical_analiz.h"
#include "syntax_analiz.h"
#include "generator.h"
#include "iostream"

class Translator {
public:
	Translator();
	void translateToTokens();
	void syntaxAnalize();
	void generateAsm();
	void logTables();
	void logTokens();

private:
	Token lexemeToToken(STATE, std::string);
	bool needToToken(STATE);

private:
	LexicalAnaliz lexAnaliz;
	SyntaxAnaliz synAnaliz;
	Generator generator;

//public:
	ConstTable keyWords;
	ConstTable signOfOperations;
	ConstTable delimeters;

	ChangedTable identificators;
	ChangedTable consts;

private:
	std::vector<Token> tokens;
};
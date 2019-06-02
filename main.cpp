#include "pch.h"
#include "iostream"
#include "table.h"
#include "translator.h"


void main() {

	int a = 10;
	int b = 23;

	int e = 89;

	int q = 12 < 3 < 10 - 9;

	try {
		Translator T;
		T.translateToTokens();
		T.syntaxAnalize();
		T.generateAsm();
	}
	catch (std::string e) {
		std::cout << e.c_str();
	}
}
#include "pch.h"
#include "iostream"
#include "table.h"
#include "translator.h"

void main() {
	try {
		Translator T;
		T.translateToTokens();
	}
	catch (std::string e) {
		std::cout << e.c_str();
	}
}
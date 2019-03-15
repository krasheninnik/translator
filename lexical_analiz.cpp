#include "pch.h"
#include "lexical_analiz.h"
#include <ctype.h>
#include <string>
#include <fstream>

LexicalAnaliz::LexicalAnaliz() {
	const double StateAmount = STATE_SIZES;
	const double TranitionAmount = EVENT_SIZE;

	// initiation of transiton matrix:
	transitionMatrix = std::vector<std::vector<STATE>>(StateAmount);
	for (auto& state : transitionMatrix) state = std::vector<STATE>(TranitionAmount);

#pragma region InitiationTransitionMatrix
	transitionMatrix[START][LETTER] = IDENTIFICATOR;
	transitionMatrix[START][DECIMAL] = INT;
	transitionMatrix[START][SLASH] = ONE_SLASH;
	transitionMatrix[START][ASTERISK] = ERROR;
	transitionMatrix[START][QUOTE] = OPEN_QUOTE;
	transitionMatrix[START][BACKSLASH] = ERROR;
	transitionMatrix[START][DELIMETER_SKIP_EV] = DELIMETER_SKIP;
	transitionMatrix[START][DELIMETER_EV] = DELIMETER;
	transitionMatrix[START][NEW_LINE] = OUT;  //// WTF  there should be?
	transitionMatrix[START][INVALID_CHARACTER] = ERROR;
	transitionMatrix[START][OPERATION_LEFT] = OPERATION2LEFT;
	transitionMatrix[START][OPERATION_RIGHT] = OPERATION2RIGHT;
	transitionMatrix[START][OPERTAION_EV] = OPERTAION;

	transitionMatrix[IDENTIFICATOR][LETTER] = IDENTIFICATOR;
	transitionMatrix[IDENTIFICATOR][DECIMAL] = IDENTIFICATOR;
	transitionMatrix[IDENTIFICATOR][SLASH] = OUT;
	transitionMatrix[IDENTIFICATOR][ASTERISK] = ERROR;
	transitionMatrix[IDENTIFICATOR][QUOTE] = ERROR;
	transitionMatrix[IDENTIFICATOR][BACKSLASH] = ERROR;
	transitionMatrix[IDENTIFICATOR][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[IDENTIFICATOR][DELIMETER_EV] = OUT;
	transitionMatrix[IDENTIFICATOR][NEW_LINE] = OUT;
	transitionMatrix[IDENTIFICATOR][INVALID_CHARACTER] = ERROR;
	transitionMatrix[IDENTIFICATOR][OPERATION_LEFT] = OUT;
	transitionMatrix[IDENTIFICATOR][OPERATION_RIGHT] = OUT;
	transitionMatrix[IDENTIFICATOR][OPERTAION_EV] = OUT;

	transitionMatrix[INT][LETTER] = ERROR;
	transitionMatrix[INT][DECIMAL] = INT;
	transitionMatrix[INT][SLASH] = OUT;
	transitionMatrix[INT][ASTERISK] = ERROR;
	transitionMatrix[INT][QUOTE] = ERROR;
	transitionMatrix[INT][BACKSLASH] = ERROR;
	transitionMatrix[INT][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[INT][DELIMETER_EV] = OUT;
	transitionMatrix[INT][NEW_LINE] = OUT;
	transitionMatrix[INT][INVALID_CHARACTER] = ERROR;
	transitionMatrix[INT][OPERATION_LEFT] = OUT;
	transitionMatrix[INT][OPERATION_RIGHT] = OUT;
	transitionMatrix[INT][OPERTAION_EV] = OUT;

	transitionMatrix[OPEN_QUOTE][LETTER] = CHAR;
	transitionMatrix[OPEN_QUOTE][DECIMAL] = CHAR;
	transitionMatrix[OPEN_QUOTE][SLASH] = CHAR;
	transitionMatrix[OPEN_QUOTE][ASTERISK] = CHAR;
	transitionMatrix[OPEN_QUOTE][QUOTE] = ERROR;
	transitionMatrix[OPEN_QUOTE][BACKSLASH] = SPEC_SYMBOL;
	transitionMatrix[OPEN_QUOTE][DELIMETER_SKIP_EV] = CHAR;
	transitionMatrix[OPEN_QUOTE][DELIMETER_EV] = CHAR;
	transitionMatrix[OPEN_QUOTE][NEW_LINE] = ERROR;
	transitionMatrix[OPEN_QUOTE][INVALID_CHARACTER] = CHAR;
	transitionMatrix[OPEN_QUOTE][OPERATION_LEFT] = CHAR;
	transitionMatrix[OPEN_QUOTE][OPERATION_RIGHT] = CHAR;
	transitionMatrix[OPEN_QUOTE][OPERTAION_EV] = CHAR;

	transitionMatrix[CHAR][LETTER] = ERROR;
	transitionMatrix[CHAR][DECIMAL] = ERROR;
	transitionMatrix[CHAR][SLASH] = ERROR;
	transitionMatrix[CHAR][ASTERISK] = ERROR;
	transitionMatrix[CHAR][QUOTE] = CLOSE_QUOTE;
	transitionMatrix[CHAR][BACKSLASH] = ERROR;
	transitionMatrix[CHAR][DELIMETER_SKIP_EV] = ERROR;
	transitionMatrix[CHAR][DELIMETER_EV] = ERROR;
	transitionMatrix[CHAR][NEW_LINE] = ERROR;
	transitionMatrix[CHAR][INVALID_CHARACTER] = ERROR;
	transitionMatrix[CHAR][OPERATION_LEFT] = ERROR;
	transitionMatrix[CHAR][OPERATION_RIGHT] = ERROR;
	transitionMatrix[CHAR][OPERTAION_EV] = ERROR;

	transitionMatrix[CLOSE_QUOTE][LETTER] = ERROR;
	transitionMatrix[CLOSE_QUOTE][DECIMAL] = ERROR;
	transitionMatrix[CLOSE_QUOTE][SLASH] = OUT;
	transitionMatrix[CLOSE_QUOTE][ASTERISK] = ERROR;
	transitionMatrix[CLOSE_QUOTE][QUOTE] = ERROR;
	transitionMatrix[CLOSE_QUOTE][BACKSLASH] = ERROR;
	transitionMatrix[CLOSE_QUOTE][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[CLOSE_QUOTE][DELIMETER_EV] = OUT;
	transitionMatrix[CLOSE_QUOTE][NEW_LINE] = OUT;
	transitionMatrix[CLOSE_QUOTE][INVALID_CHARACTER] = ERROR;
	transitionMatrix[CLOSE_QUOTE][OPERATION_LEFT] = OUT;
	transitionMatrix[CLOSE_QUOTE][OPERATION_RIGHT] = OUT;
	transitionMatrix[CLOSE_QUOTE][OPERTAION_EV] = OUT;


	transitionMatrix[BACKSLACH_SPEC_SYMBOL][LETTER] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][DECIMAL] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][SLASH] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][ASTERISK] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][QUOTE] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][BACKSLASH] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][DELIMETER_SKIP_EV] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][DELIMETER_EV] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][NEW_LINE] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][INVALID_CHARACTER] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][OPERATION_LEFT] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][OPERATION_RIGHT] = SPEC_SYMBOL;
	transitionMatrix[BACKSLACH_SPEC_SYMBOL][OPERTAION_EV] = SPEC_SYMBOL;

	transitionMatrix[SPEC_SYMBOL][LETTER] = ERROR;
	transitionMatrix[SPEC_SYMBOL][DECIMAL] = ERROR;
	transitionMatrix[SPEC_SYMBOL][SLASH] = ERROR;
	transitionMatrix[SPEC_SYMBOL][ASTERISK] = ERROR;
	transitionMatrix[SPEC_SYMBOL][QUOTE] = OUT;
	transitionMatrix[SPEC_SYMBOL][BACKSLASH] = ERROR;
	transitionMatrix[SPEC_SYMBOL][DELIMETER_SKIP_EV] = ERROR;
	transitionMatrix[SPEC_SYMBOL][DELIMETER_EV] = ERROR;
	transitionMatrix[SPEC_SYMBOL][NEW_LINE] = ERROR;
	transitionMatrix[SPEC_SYMBOL][INVALID_CHARACTER] = ERROR;
	transitionMatrix[SPEC_SYMBOL][OPERATION_LEFT] = ERROR;
	transitionMatrix[SPEC_SYMBOL][OPERATION_RIGHT] = ERROR;
	transitionMatrix[SPEC_SYMBOL][OPERTAION_EV] = ERROR;

	transitionMatrix[ONE_SLASH][LETTER] = ERROR;
	transitionMatrix[ONE_SLASH][DECIMAL] = ERROR;
	transitionMatrix[ONE_SLASH][SLASH] = ONESTRING_COMMENT;
	transitionMatrix[ONE_SLASH][ASTERISK] = COMMENT;
	transitionMatrix[ONE_SLASH][QUOTE] = ERROR;
	transitionMatrix[ONE_SLASH][BACKSLASH] = ERROR;
	transitionMatrix[ONE_SLASH][DELIMETER_SKIP_EV] = ERROR;
	transitionMatrix[ONE_SLASH][DELIMETER_EV] = ERROR;
	transitionMatrix[ONE_SLASH][NEW_LINE] = ERROR;
	transitionMatrix[ONE_SLASH][INVALID_CHARACTER] = ERROR;
	transitionMatrix[ONE_SLASH][OPERATION_LEFT] = ERROR;
	transitionMatrix[ONE_SLASH][OPERATION_RIGHT] = ERROR;
	transitionMatrix[ONE_SLASH][OPERTAION_EV] = ERROR;

	transitionMatrix[ONESTRING_COMMENT][LETTER] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][DECIMAL] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][SLASH] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][ASTERISK] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][QUOTE] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][BACKSLASH] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][DELIMETER_SKIP_EV] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][DELIMETER_EV] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][NEW_LINE] = OUT;
	transitionMatrix[ONESTRING_COMMENT][INVALID_CHARACTER] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][OPERATION_LEFT] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][OPERATION_RIGHT] = ONESTRING_COMMENT;
	transitionMatrix[ONESTRING_COMMENT][OPERTAION_EV] = ONESTRING_COMMENT;
	
	transitionMatrix[COMMENT][LETTER] = COMMENT;
	transitionMatrix[COMMENT][DECIMAL] = COMMENT;
	transitionMatrix[COMMENT][SLASH] = COMMENT;
	transitionMatrix[COMMENT][ASTERISK] = CLOSED_ASTERISK;
	transitionMatrix[COMMENT][QUOTE] = COMMENT;
	transitionMatrix[COMMENT][BACKSLASH] = COMMENT;
	transitionMatrix[COMMENT][DELIMETER_SKIP_EV] = COMMENT;
	transitionMatrix[COMMENT][DELIMETER_EV] = COMMENT;
	transitionMatrix[COMMENT][NEW_LINE] = COMMENT;
	transitionMatrix[COMMENT][INVALID_CHARACTER] = COMMENT;
	transitionMatrix[COMMENT][OPERATION_LEFT] = COMMENT;
	transitionMatrix[COMMENT][OPERATION_RIGHT] = COMMENT;
	transitionMatrix[COMMENT][OPERTAION_EV] = COMMENT;

	transitionMatrix[CLOSED_ASTERISK][LETTER] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][DECIMAL] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][SLASH] = CLOSED_COMMENT;
	transitionMatrix[CLOSED_ASTERISK][ASTERISK] = CLOSED_ASTERISK;
	transitionMatrix[CLOSED_ASTERISK][QUOTE] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][BACKSLASH] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][DELIMETER_SKIP_EV] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][DELIMETER_EV] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][NEW_LINE] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][INVALID_CHARACTER] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][OPERATION_LEFT] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][OPERATION_RIGHT] = COMMENT;
	transitionMatrix[CLOSED_ASTERISK][OPERTAION_EV] = COMMENT;

	transitionMatrix[CLOSED_COMMENT][LETTER] = OUT;
	transitionMatrix[CLOSED_COMMENT][DECIMAL] = OUT;
	transitionMatrix[CLOSED_COMMENT][SLASH] = OUT;
	transitionMatrix[CLOSED_COMMENT][ASTERISK] = OUT;
	transitionMatrix[CLOSED_COMMENT][QUOTE] = OUT;
	transitionMatrix[CLOSED_COMMENT][BACKSLASH] = OUT;
	transitionMatrix[CLOSED_COMMENT][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[CLOSED_COMMENT][DELIMETER_EV] = OUT;
	transitionMatrix[CLOSED_COMMENT][NEW_LINE] = OUT;
	transitionMatrix[CLOSED_COMMENT][INVALID_CHARACTER] = OUT;
	transitionMatrix[CLOSED_COMMENT][OPERATION_LEFT] = OUT;
	transitionMatrix[CLOSED_COMMENT][OPERATION_RIGHT] = OUT;
	transitionMatrix[CLOSED_COMMENT][OPERTAION_EV] = OUT;

	transitionMatrix[OPERTAION][LETTER] = OUT;
	transitionMatrix[OPERTAION][DECIMAL] = OUT;
	transitionMatrix[OPERTAION][SLASH] = OUT;
	transitionMatrix[OPERTAION][ASTERISK] = ERROR;
	transitionMatrix[OPERTAION][QUOTE] = OUT;
	transitionMatrix[OPERTAION][BACKSLASH] = ERROR;
	transitionMatrix[OPERTAION][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[OPERTAION][DELIMETER_EV] = OUT;
	transitionMatrix[OPERTAION][NEW_LINE] = OUT;
	transitionMatrix[OPERTAION][INVALID_CHARACTER] = ERROR;
	transitionMatrix[OPERTAION][OPERATION_LEFT] = OUT;
	transitionMatrix[OPERTAION][OPERATION_RIGHT] = OUT;
	transitionMatrix[OPERTAION][OPERTAION_EV] = OUT;

	transitionMatrix[OPERATION2LEFT][LETTER] = OUT;
	transitionMatrix[OPERATION2LEFT][DECIMAL] = OUT;
	transitionMatrix[OPERATION2LEFT][SLASH] = OUT;
	transitionMatrix[OPERATION2LEFT][ASTERISK] = ERROR;
	transitionMatrix[OPERATION2LEFT][QUOTE] = OUT;
	transitionMatrix[OPERATION2LEFT][BACKSLASH] = ERROR;
	transitionMatrix[OPERATION2LEFT][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[OPERATION2LEFT][DELIMETER_EV] = OUT;
	transitionMatrix[OPERATION2LEFT][NEW_LINE] = OUT;
	transitionMatrix[OPERATION2LEFT][INVALID_CHARACTER] = OUT;
	transitionMatrix[OPERATION2LEFT][OPERATION_LEFT] = OPERATION2LEFT2;
	transitionMatrix[OPERATION2LEFT][OPERATION_RIGHT] = OUT;
	transitionMatrix[OPERATION2LEFT][OPERTAION_EV] = OUT;

	transitionMatrix[OPERATION2LEFT2][LETTER] = OUT;
	transitionMatrix[OPERATION2LEFT2][DECIMAL] = OUT;
	transitionMatrix[OPERATION2LEFT2][SLASH] = OUT;
	transitionMatrix[OPERATION2LEFT2][ASTERISK] = ERROR;
	transitionMatrix[OPERATION2LEFT2][QUOTE] = OUT;
	transitionMatrix[OPERATION2LEFT2][BACKSLASH] = ERROR;
	transitionMatrix[OPERATION2LEFT2][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[OPERATION2LEFT2][DELIMETER_EV] = OUT;
	transitionMatrix[OPERATION2LEFT2][NEW_LINE] = OUT;
	transitionMatrix[OPERATION2LEFT2][INVALID_CHARACTER] = ERROR;
	transitionMatrix[OPERATION2LEFT2][OPERATION_LEFT] = OUT;
	transitionMatrix[OPERATION2LEFT2][OPERATION_RIGHT] = OUT;
	transitionMatrix[OPERATION2LEFT2][OPERTAION_EV] = OUT;

	transitionMatrix[OPERATION2RIGHT][LETTER] = OUT;
	transitionMatrix[OPERATION2RIGHT][DECIMAL] = OUT;
	transitionMatrix[OPERATION2RIGHT][SLASH] = OUT;
	transitionMatrix[OPERATION2RIGHT][ASTERISK] = ERROR;
	transitionMatrix[OPERATION2RIGHT][QUOTE] = OUT;
	transitionMatrix[OPERATION2RIGHT][BACKSLASH] = ERROR;
	transitionMatrix[OPERATION2RIGHT][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[OPERATION2RIGHT][DELIMETER_EV] = OUT;
	transitionMatrix[OPERATION2RIGHT][NEW_LINE] = OUT;
	transitionMatrix[OPERATION2RIGHT][INVALID_CHARACTER] = OUT;
	transitionMatrix[OPERATION2RIGHT][OPERATION_RIGHT] = OUT;
	transitionMatrix[OPERATION2RIGHT][OPERATION_RIGHT] = OPERATION2RIGHT2;
	transitionMatrix[OPERATION2RIGHT][OPERTAION_EV] = OUT;

	transitionMatrix[OPERATION2RIGHT2][LETTER] = OUT;
	transitionMatrix[OPERATION2RIGHT2][DECIMAL] = OUT;
	transitionMatrix[OPERATION2RIGHT2][SLASH] = OUT;
	transitionMatrix[OPERATION2RIGHT2][ASTERISK] = ERROR;
	transitionMatrix[OPERATION2RIGHT2][QUOTE] = OUT;
	transitionMatrix[OPERATION2RIGHT2][BACKSLASH] = ERROR;
	transitionMatrix[OPERATION2RIGHT2][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[OPERATION2RIGHT2][DELIMETER_EV] = OUT;
	transitionMatrix[OPERATION2RIGHT2][NEW_LINE] = OUT;
	transitionMatrix[OPERATION2RIGHT2][INVALID_CHARACTER] = ERROR;
	transitionMatrix[OPERATION2RIGHT2][OPERATION_RIGHT] = OUT;
	transitionMatrix[OPERATION2RIGHT2][OPERATION_RIGHT] = OUT;
	transitionMatrix[OPERATION2RIGHT2][OPERTAION_EV] = OUT;

	transitionMatrix[DELIMETER_SKIP][LETTER] = OUT;
	transitionMatrix[DELIMETER_SKIP][DECIMAL] = OUT;
	transitionMatrix[DELIMETER_SKIP][SLASH] = OUT;
	transitionMatrix[DELIMETER_SKIP][ASTERISK] = OUT;
	transitionMatrix[DELIMETER_SKIP][QUOTE] = OUT;
	transitionMatrix[DELIMETER_SKIP][BACKSLASH] = OUT;
	transitionMatrix[DELIMETER_SKIP][DELIMETER_SKIP_EV] = DELIMETER_SKIP;
	transitionMatrix[DELIMETER_SKIP][DELIMETER_EV] = OUT;
	transitionMatrix[DELIMETER_SKIP][NEW_LINE] = OUT;
	transitionMatrix[DELIMETER_SKIP][INVALID_CHARACTER] = OUT;
	transitionMatrix[DELIMETER_SKIP][OPERATION_LEFT] = OUT;
	transitionMatrix[DELIMETER_SKIP][OPERATION_RIGHT] = OUT;
	transitionMatrix[DELIMETER_SKIP][OPERTAION_EV] = OUT;

	// there is no situations where "DELIMETER" -> "SOME STATE" transitions
	transitionMatrix[DELIMETER][LETTER] = OUT;
	transitionMatrix[DELIMETER][DECIMAL] = OUT;
	transitionMatrix[DELIMETER][SLASH] = OUT;
	transitionMatrix[DELIMETER][ASTERISK] = OUT;
	transitionMatrix[DELIMETER][QUOTE] = OUT;
	transitionMatrix[DELIMETER][BACKSLASH] = OUT;
	transitionMatrix[DELIMETER][DELIMETER_SKIP_EV] = OUT;
	transitionMatrix[DELIMETER][DELIMETER_EV] = OUT;
	transitionMatrix[DELIMETER][NEW_LINE] = OUT;
	transitionMatrix[DELIMETER][INVALID_CHARACTER] = OUT;
	transitionMatrix[DELIMETER][OPERATION_LEFT] = OUT;
	transitionMatrix[DELIMETER][OPERATION_RIGHT] = OUT;
	transitionMatrix[DELIMETER][OPERTAION_EV] = OUT;


	transitionMatrix[ERROR][LETTER] = ERROR;
	transitionMatrix[ERROR][DECIMAL] = ERROR;
	transitionMatrix[ERROR][SLASH] = ERROR;
	transitionMatrix[ERROR][ASTERISK] = ERROR;
	transitionMatrix[ERROR][QUOTE] = ERROR;
	transitionMatrix[ERROR][BACKSLASH] = ERROR;
	transitionMatrix[ERROR][DELIMETER_SKIP_EV] = ERROR;
	transitionMatrix[ERROR][DELIMETER_EV] = ERROR;
	transitionMatrix[ERROR][NEW_LINE] = OUT;
	transitionMatrix[ERROR][INVALID_CHARACTER] = ERROR;
	transitionMatrix[ERROR][OPERATION_LEFT] = ERROR;
	transitionMatrix[ERROR][OPERATION_RIGHT] = ERROR;
	transitionMatrix[ERROR][OPERTAION_EV] = ERROR;
				
	// there is no situations where "OUT" -> "SOME STATE" transitions
	transitionMatrix[OUT][LETTER] = START;
	transitionMatrix[OUT][DECIMAL] = START;
	transitionMatrix[OUT][SLASH] = START;
	transitionMatrix[OUT][ASTERISK] = START;
	transitionMatrix[OUT][QUOTE] = START;
	transitionMatrix[OUT][BACKSLASH] = START;
	transitionMatrix[OUT][DELIMETER_SKIP_EV] = START;
	transitionMatrix[OUT][DELIMETER_EV] = START;
	transitionMatrix[OUT][NEW_LINE] = START;
	transitionMatrix[OUT][INVALID_CHARACTER] = START;
	transitionMatrix[OUT][OPERATION_LEFT] = START;
	transitionMatrix[OUT][OPERATION_RIGHT] = START;
	transitionMatrix[OUT][OPERTAION_EV] = START;
#pragma endregion
}

#pragma region logs
void LexicalAnaliz::errorLog(std::ofstream& ferr, const int row, const std::string& lexeme) {
	ferr << row << " line, error in lexeme: " << lexeme << std::endl;
}

#pragma endregion


EVENT LexicalAnaliz::recognizeEvent(char symbol) {
	EVENT event;

	if (isalpha(symbol)) { event = LETTER; return event; }
	if (isdigit(symbol)) { event = DECIMAL; return event; }

	switch (symbol) {
	// COMMENTS:
	case '/':  event = SLASH; break;
	case '*':  event = ASTERISK; break;

	// CHAR:
	case '\'': event = QUOTE; break;
	case '\\': event = BACKSLASH; break;
	
	// DELIMETERS:
	case '\n': event = NEW_LINE; break;
	case ',':
	case ';':  event = DELIMETER_EV; break;
	case ' ':
	case '\t': event = DELIMETER_SKIP_EV; break;

	// OPERTAIONS:
	case '=':
	case '+':
	case '-':
	case '&':
	case '|': event = OPERTAION_EV; break;
	case '<':  event = OPERATION_LEFT; break;
	case '>':  event = OPERATION_RIGHT; break;

	// INVALID
	default:  event = INVALID_CHARACTER; break;
	}

	return event;
}


STATE  LexicalAnaliz::transition(STATE state, EVENT event) {
//#ifdef _DEBUG
//	if (transitionMatrix[state][event] == STATE::START) throw "Somthing wrong. Transition to Start";
//#endif
	return transitionMatrix[state][event];
}
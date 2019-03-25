#pragma once
#include <vector>

enum STATE {
	START = 0,			// +
/////////////////////////////
	IDENTIFICATOR,		// +
	INT,				// 
	OPEN_QUOTE,			// 
	CHAR,				//
	CLOSE_QUOTE,		
	WAIT_SPEC_SYMBOL,
	SPEC_SYMBOL,		// 
/////////////////////////////	
	ONE_SLASH,		    // 
	ONESTRING_COMMENT,  // 
	COMMENT,			// 
	CLOSED_ASTERISK,	// 
	CLOSED_COMMENT,		//
//////////////////////////////	
	OPERTAION,
	OPERATION2LEFT,		// 
	OPERATION2LEFT2,		
	OPERATION2RIGHT,	// 
	OPERATION2RIGHT2,	
///////////////////////////////
    DELIMETER_SKIP,		// that you can   skip
	DELIMETER,			// that you can't skip
//////////////////////////////
	ERROR,					// 
	OUT,					// fictitious state
	STATE_SIZES
};

enum EVENT {
	LETTER = 1,
	DECIMAL,
/////////////////
	SLASH,
	ASTERISK,
/////////////////
	QUOTE,
	BACKSLASH,
/////////////////
	DELIMETER_SKIP_EV,
	DELIMETER_EV,
	NEW_LINE,
////////////////
	INVALID_CHARACTER,
////////////////
	OPERATION_LEFT,
	OPERATION_RIGHT,
	OPERTAION_EV,
////////////////////
	EVENT_SIZE
};


/*
need output:
	error's log
	file of tokens
	content of changed tables
*/

class LexicalAnaliz {
public:
	LexicalAnaliz();
	EVENT recognizeEvent(char symbol);
	STATE transition(STATE state, EVENT event);
	void errorLog(std::ofstream& err, int row, const std::string& lexeme);

private:
	std::vector<std::vector<STATE>> transitionMatrix;
};
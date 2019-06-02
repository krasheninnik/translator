#pragma once
#include <functional>
#include <vector>
#include "header.h"

enum TABLE {
	KEYWORDS,
	OPERATIONS,
	DELIMETERS,
	IDENTIFIATORS,
	CONSTS
};

struct Token {
	TABLE table;
	int place;
	int place2;
};

struct Info {
	std::string name;
	TYPE type;
	bool initializated;
};

class ConstTable {	// ordered table
public:
	ConstTable() = default;
	ConstTable(TABLE, std::vector<std::string>&);
	Token find(const std::string&);
	
	std::string operator[] (int place) { return elems[place]; };
	std::string getName(int place) { return elems[place]; };


private:
	TABLE tableNum;
	std::vector<std::string> elems;
};

class ChangedTable { // hash table
public:
	ChangedTable() = default;
	ChangedTable(TABLE tableNumber, int size);
	Token findAdd(const std::string&);
	void logTable(std::ofstream&);

	std::string getName(int place, int place2);

	TYPE getType(int place, int place2);
	void setType(int place, int place2, TYPE type);

	bool getInit(int place, int place2);
	void setInit(int place, int place2, bool mode);

	std::string dataAllocation();
	std::vector<std::vector<Info>> getElems();

private:
	std::function<size_t(const std::string&)> hashFn;
	std::vector<std::vector<Info>> elems;
	TABLE tableNum;
};

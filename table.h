#pragma once
#include <functional>
#include <vector>

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
	int type;
	int c;
};

class ConstTable {	// ordered table
public:
	ConstTable() = default;
	ConstTable(TABLE, std::vector<std::string>&);
	Token find(const std::string&);

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
	int getType(int place, int place2);
	void setType(int place, int place2, int type);

private:
	std::function<size_t(const std::string&)> hashFn;
	std::vector<std::vector<Info>> elems;
	TABLE tableNum;
};

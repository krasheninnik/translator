#include "pch.h"
#include "table.h"
#include <algorithm>
#include <string>
#include <fstream>
#include "syntax_analiz.h"

ConstTable::ConstTable(TABLE tableNumber, std::vector<std::string> &init_list) {
	elems.swap(init_list);
	std::sort(elems.begin(), elems.end());

	tableNum = tableNumber;
}

Token ConstTable::find(const std::string& key) {
	// binary search
	int left = 0, right = elems.size();
	int place;

	while(left != right) {
		place = (right + left) / 2;

		if (elems[place] == key) break;
		if (elems[place] > key) right = place;
		else left = place + 1;
	}

	return { tableNum, elems[place] == key ? place : -1, 0 };
}

ChangedTable::ChangedTable(TABLE tableNumber, int size) {
	tableNum = tableNumber;
	elems = std::vector<std::vector<Info>>(size);

	std::hash<std::string> a;		// init hash_fn
	hashFn = a;
}

Token ChangedTable::findAdd(const std::string& key) {

	int place = hashFn(key) % elems.size();
	int place2 = 0;

	if (elems[place].empty()) elems[place].push_back(Info{key, TYPE::UNDEFINED, false });
	else {
		bool finded = false;
		for (auto el : elems[place]) {
			if (el.name == key) finded = true;
			else place2++;
		}

		if (!finded) elems[place].push_back(Info{ key, TYPE::UNDEFINED, false });
	}

	return { tableNum, place, place2 };
}

std::string ChangedTable::ChangedTable::getName(int place, int place2) { // change to token
	return elems[place][place2].name;
}

TYPE ChangedTable::ChangedTable::getType(int place, int place2) {
	return elems[place][place2].type;
}

void ChangedTable::setType(int place, int place2, TYPE newType) {
	elems[place][place2].type = newType;
}


bool  ChangedTable::getInit(int place, int place2) {
	return elems[place][place2].initializated;
}
void  ChangedTable::setInit(int place, int place2, bool mode) {
	elems[place][place2].initializated = mode;
}

void ChangedTable::logTable(std::ofstream& fout) {
	for (int i = 0; i < elems.size(); i++) {
		for (int j = 0; j < elems[i].size(); j++) {
			fout << elems[i][j].name << std::endl;
		}
	}
}


std::string ChangedTable::dataAllocation() {
	std::string res = "";
	for (int i = 0; i < elems.size(); i++) {
		for (int j = 0; j < elems[i].size(); j++) {
			// formating string of code to memory allocation
			auto& elem = elems[i][j];

			switch (elem.type) {
			case TYPE::CHAR: res += elem.name + " db ?\n"; break; // DB Define Byte, allocates 1 byte
			case TYPE::INT: res += elem.name + " dd ?\n"; break; // DD Define Doubleword, allocates 4 bytes
			}
		}
	}
	return res;
}

std::vector<std::vector<Info>> ChangedTable::getElems() {
	return elems;
}
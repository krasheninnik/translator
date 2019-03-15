#include "pch.h"
#pragma region TestBinarySearch
/*
std::vector<std::string> init_table1_list{
		"a",
		"b",
		"c",
		"d",
		"e" };

	ConstTable t(init_table1_list);

	std::cout << t.find("a") << std::endl;
	std::cout << t.find("b") << std::endl;
	std::cout << t.find("c") << std::endl;
	std::cout << t.find("d") << std::endl;
	std::cout << t.find("e") << std::endl;
	std::cout << t.find("f") << std::endl;

	std::vector<std::string> init_table1_list2{
				"a",
				"b",
				"c",
				"d",
				"e",
				"f" };

	ConstTable t2(init_table1_list2);

	std::cout << std::endl;
	std::cout << t2.find("a") << std::endl;
	std::cout << t2.find("b") << std::endl;
	std::cout << t2.find("c") << std::endl;
	std::cout << t2.find("d") << std::endl;
	std::cout << t2.find("e") << std::endl;
	std::cout << t2.find("f") << std::endl;
	std::cout << t2.find("g") << std::endl;


	std::vector<std::string> init_table1_list3{ "a"	};

	ConstTable t3(init_table1_list3);
	std::cout <<  std::endl;
	std::cout << t3.find("a") << std::endl;
	std::cout << t3.find("b") << std::endl;
*/
#pragma endregion
#pragma region TestHashTable
/*
	ChangedTable t(0, 2);
	auto t1 = t.findAdd("a");
	auto t2 = t.findAdd("b");
	auto t3 = t.findAdd("c");
	auto t4 = t.findAdd("d");
	auto t5 = t.findAdd("e");
	auto t6 = t.findAdd("f");

	std::cout << "Table\tPlace\tPlace2" << std::endl;
	std::cout << t1.table << "\t" << t1.place << "\t" << t1.place2 << std::endl;
	std::cout << t2.table << "\t" << t2.place << "\t" << t2.place2 << std::endl;
	std::cout << t3.table << "\t" << t3.place << "\t" << t3.place2 << std::endl;
	std::cout << t4.table << "\t" << t4.place << "\t" << t4.place2 << std::endl;
	std::cout << t5.table << "\t" << t5.place << "\t" << t5.place2 << std::endl;
	std::cout << t6.table << "\t" << t6.place << "\t" << t6.place2 << std::endl;


	ChangedTable tt(0, 700);
	auto t11 = tt.findAdd("a");
	auto t22 = tt.findAdd("b");
	auto t33 = tt.findAdd("c");
	auto t44 = tt.findAdd("d");
	auto t55 = tt.findAdd("e");
	auto t66 = tt.findAdd("f");

	std::cout << "Table\tPlace\tPlace2" << std::endl;
	std::cout << t11.table << "\t" << t11.place << "\t" << t11.place2 << std::endl;
	std::cout << t22.table << "\t" << t22.place << "\t" << t22.place2 << std::endl;
	std::cout << t33.table << "\t" << t33.place << "\t" << t33.place2 << std::endl;
	std::cout << t44.table << "\t" << t44.place << "\t" << t44.place2 << std::endl;
	std::cout << t55.table << "\t" << t55.place << "\t" << t55.place2 << std::endl;
	std::cout << t66.table << "\t" << t66.place << "\t" << t66.place2 << std::endl;

	std::cout << "Set/get test: << std::endl;
	std::cout << "Name: " << t.getName(t1.place, t1.place2).c_str() <<
				 " type: " << t.getType(t1.place, t1.place2);
	t.setType(t1.place, t1.place2, 3);
	std::cout << " new type: " << t.getType(t1.place, t1.place2) << std::endl;
*/
#pragma endregion

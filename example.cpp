#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct translator { bool parse(string, string); };
struct token {};

bool translator::parse(string tokens_file, string errors_file) {

	ifstream parse_token_f(tokens_file.c_str()); //поток для ввода токенов
	ofstream parse_error_f(errors_file.c_str()); //поток для вывода ошибок

	token cur_t, next_t; //текущий токен и следующий за ним

	bool local_error = false; //наличие ошибки

	parse_token_f >> cur_t;
	parse_token_f >> next_t;

	int cur_row = 0; //текущая строка в таблице разбора
	int prev_row; //предыдущая строка в таблице разбора

	bool have_type = false; //определяем ли мы сейчас тип
	int type_type; //опеределяемый тип

	//переменные для построения дерева разбора

	bool little_tree_bg = false; //начато ли построение малого дерева
	string token_str_prev; //терминал предыдущего токена
	int id_numb; //номер встречаемого индетификатора

	vector<token> little_tree_code; // токены для создания маленького дерева

	while (!parse_token_f.eof() && !local_error) {

		string token_str = get_token_text(cur_t); // какой текст содержится в токене
		if (cur_t.table_n == 5 || cur_t.table_n == 6)
			token_str = "ID";

		if (token_str == "ID")
			little_tree_bg = true;

		bool find_terminal = false; //допустим ли данный терминал

		for (int i = 0; i < parsing_table[cur_row].termenal.size() & !find_terminal; i++) {
			if (parsing_table[cur_row].termenal[i] == token_str)
				find_terminal = true;
		}

		if (find_terminal) { //если получаем то, что ожидали то обрабатываем это

			bool change_row = false; //сменили ли мы строку

			if (parsing_table[cur_row].put_in_stack)
				parsing_stack.push(cur_row + 1); //если надо получить в стек - ложим

			if (parsing_table[cur_row].accept) { //принимаем терминал и если надо - расширяем дерево

				if (little_tree_bg) {
					little_tree_code.push_back(cur_t);
				}

				if (token_str == ";" || token_str == ",") { //если закончили разбор цельного оператора
					grow_tree(little_tree_code); //добавили всё что нужно в дерево

					//и перешли в исходное состояние
					little_tree_code.clear();
					little_tree_bg = false;

				}

				//все, обнуляем типа больше нет
				if (token_str == ";") {
					have_type = false;
				}

				//Если мы нашли тип, то мы его запоминаем
				if (token_str == "int" || token_str == "char" || token_str == "float") {
					have_type = true;
					if (token_str == "int")
						type_type = 1;
					if (token_str == "char")
						type_type = 2;
					if (token_str == "float")
						type_type = 3;
				}


				//Заносим тип в таблицу идентицикаторов
				if (token_str == "ID" && have_type && cur_row == 47) {
					identifier.set_ind_type(get_token_text(cur_t), type_type);
				}

				//Если вдруг попытались присвоить что-то константе
				if (cur_row == 30 && cur_t.table_n != 5) {
					parse_error_f << "Ошибка в обработке " << get_token_text(cur_t) << " константе не может быть присовенно значение" << endl;
					cout << "Lex error" << endl;
					local_error = true;
				}

				//и пошли дальше
				cur_t = next_t;
				if (!parse_token_f.eof())
					parse_token_f >> next_t; //если принимает, то считываем новый
			}

			if (parsing_table[cur_row].should_return) {
				prev_row = cur_row; //запоминаем предыдущий
				cur_row = parsing_stack.top(); //если надо взять из стека - берём
				parsing_stack.pop();
				change_row = true;
			}


			if (!change_row && parsing_table[cur_row].jump != -1) {
				cur_row = parsing_table[cur_row].jump; //если надо прыгнуть - прыгаем
			}

		}
		else { //если произошщло несоответсвие
			if (parsing_table[cur_row].error) { //если можем судить что уже ошибка, то возвращаем её
				local_error = true;
				parse_error_f << "Ошибка в обработке " << get_token_text(cur_t) << endl;
				cout << "Lex error" << endl;

				//Для РГЗ 1, начало - вывод альтернатив
				parse_error_f << "Возможно на этом месте должно быть: ";
				do {
					for (int i = 0; i < parsing_table[cur_row].termenal.size(); i++) {
						parse_error_f << parsing_table[cur_row].termenal[i] << " ";
					}
					cur_row--;
				} while (!parsing_table[cur_row].error);
				parse_error_f << endl;
				//Для РГЗ 1 - конец
			}
			else { //Если нет - переходим на следующий
				cur_row++;
			}
		}
		token_str_prev = token_str;
	};

	//Для РГЗ 2, начало - определения проверка определения типов
	for (int i = 10; i < 32; i++) {
		bool is_els = true; //еслить ли элементы
		int j = 0; //номер элемента в цепочке
		lexeme check_lex; //получаемая лексема
		while (is_els) {
			is_els = identifier.get_lexeme(i, j, check_lex);
			if (is_els) {
				if (check_lex.type == 0) {
					local_error = true;
					parse_error_f << "Ошибка в обработке идентификатора " << check_lex.name << " не объявлен тип" << endl;
					cout << "Type error" << endl;
				}
				j++;
			}
		};
	}

	//Для РГЗ 2 - конец

	parse_token_f.close();
	parse_error_f.close();

	return !local_error;
}

//  Представление кода smallTreeCode в виде дерева,
//  при конце разбора целостного оператора. (";" or ",");
//
void translator::grow_tree(vector<token> code) {

	tree_el* little_tree_beg = new tree_el; //корень маленького дерева

	if (code.size() > 2) {
		grow_little_tree(code, little_tree_beg); //веращиваем маленькое деревце

		//Если это самое начало дерева
		if (tree_begin == 0) {
			tree_begin = little_tree_beg;
			tree_cur = tree_begin;
		}
		else {
			tree_cur->right = little_tree_beg; //и присоединяем его к большому
		}

		while (tree_cur->right != 0) { //перемещаем курсор дерева
			tree_cur = tree_cur->right;
		};

	}

}

void translator::grow_little_tree(vector<token> code, tree_el *&beg) {

	vector<token> L, R; //левое и правое поддеревья

	int bracket_num = 0; //количество скобок
	int L_num = 0;

	bool flag = false;


	//Если последний токен
	if (code.size() == 1 && (get_token_text(code[0]) == ";" || get_token_text(code[0]) == ",")) {
		beg->id = ";";
		beg->type = 2;
	}
	else {

		vector<int> opers_n; //номера "верхних операций"

		//Находим все "верхние операции"
		for (int i = 0; i < code.size(); i++) {
			if (get_token_text(code[i]) == "(") bracket_num++;
			if (get_token_text(code[i]) == ")") bracket_num--;
			if (code[i].table_n == 2 && bracket_num == 0) opers_n.push_back(i);
		}

		if (opers_n.size() == 0) { //ели операций нет пропуска всё связанное с ними
			flag = false;
		}
		else { //если есть - ищим операцию с наименьшим приоритетом
			int low_num = -1; //номер нужной нам операции
			//Сначало проверка на наличие равенства
			for (int j = 0; j < opers_n.size() && low_num == -1; j++)
				if (get_token_text(code[opers_n[j]]) == "=" || get_token_text(code[opers_n[j]]) == "+=" || get_token_text(code[opers_n[j]]) == "-=" || get_token_text(code[opers_n[j]]) == "*=")
					low_num = opers_n[j];
			//Потом на + и -
			for (int j = opers_n.size() - 1; j >= 0 && low_num == -1; j--) {
				if (get_token_text(code[opers_n[j]]) == "+" || get_token_text(code[opers_n[j]]) == "-") {
					low_num = opers_n[j]; //нашли нашу операцию
				}
			}

			//Если не нашли выбираем последнюю операцию
			if (low_num == -1)
				low_num = opers_n[opers_n.size() - 1];

			//Привет интегрулятору

			if (get_token_text(code[0]) == "(") L_num++;

			//Формируем левое поддерево
			for (int j = 0; L_num < low_num; L_num++, j++)
				L.push_back(code[L_num]);

			//Формируем правое поддерево
			int R_num = 0;
			int l;
			if (get_token_text(code[low_num + 1]) == "(") R_num++;
			for (l = 0, R_num = low_num + 1 + R_num; R_num < code.size(); R_num++, l++)
				R.push_back(code[R_num]);

			beg->left = new tree_el;
			beg->right = new tree_el;

			beg->id = get_token_text(code[low_num]);
			beg->type = 0;

			grow_little_tree(L, beg->left);
			grow_little_tree(R, beg->right);
			flag = true;
		}

		if (!flag) {
			int k = 0;
			while (get_token_text(code[k]) == "(") k++;
			beg->id = get_token_text(code[k]);
			beg->type = 1;

			if (code.size() > k + 1) {
				beg->right = new tree_el;
				beg->right->id = ";";
				beg->right->type = 2;
			}

		}
	}

}

void translator::out_tree(string f_name) {
	ofstream out_f(f_name.c_str());

	out_tree_rec(out_f, tree_begin);

	out_f.close();
}

void translator::out_tree_rec(ofstream& out_f, tree_el *beg) {
	if (beg != 0) {
		if (beg->left != 0) out_tree_rec(out_f, beg->left);
		if (beg->right != 0) out_tree_rec(out_f, beg->right);

		out_f << beg->id << " ";
	}
}

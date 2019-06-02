#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct translator { bool parse(string, string); };
struct token {};

bool translator::parse(string tokens_file, string errors_file) {

	ifstream parse_token_f(tokens_file.c_str()); //����� ��� ����� �������
	ofstream parse_error_f(errors_file.c_str()); //����� ��� ������ ������

	token cur_t, next_t; //������� ����� � ��������� �� ���

	bool local_error = false; //������� ������

	parse_token_f >> cur_t;
	parse_token_f >> next_t;

	int cur_row = 0; //������� ������ � ������� �������
	int prev_row; //���������� ������ � ������� �������

	bool have_type = false; //���������� �� �� ������ ���
	int type_type; //������������� ���

	//���������� ��� ���������� ������ �������

	bool little_tree_bg = false; //������ �� ���������� ������ ������
	string token_str_prev; //�������� ����������� ������
	int id_numb; //����� ������������ ��������������

	vector<token> little_tree_code; // ������ ��� �������� ���������� ������

	while (!parse_token_f.eof() && !local_error) {

		string token_str = get_token_text(cur_t); // ����� ����� ���������� � ������
		if (cur_t.table_n == 5 || cur_t.table_n == 6)
			token_str = "ID";

		if (token_str == "ID")
			little_tree_bg = true;

		bool find_terminal = false; //�������� �� ������ ��������

		for (int i = 0; i < parsing_table[cur_row].termenal.size() & !find_terminal; i++) {
			if (parsing_table[cur_row].termenal[i] == token_str)
				find_terminal = true;
		}

		if (find_terminal) { //���� �������� ��, ��� ������� �� ������������ ���

			bool change_row = false; //������� �� �� ������

			if (parsing_table[cur_row].put_in_stack)
				parsing_stack.push(cur_row + 1); //���� ���� �������� � ���� - �����

			if (parsing_table[cur_row].accept) { //��������� �������� � ���� ���� - ��������� ������

				if (little_tree_bg) {
					little_tree_code.push_back(cur_t);
				}

				if (token_str == ";" || token_str == ",") { //���� ��������� ������ �������� ���������
					grow_tree(little_tree_code); //�������� �� ��� ����� � ������

					//� ������� � �������� ���������
					little_tree_code.clear();
					little_tree_bg = false;

				}

				//���, �������� ���� ������ ���
				if (token_str == ";") {
					have_type = false;
				}

				//���� �� ����� ���, �� �� ��� ����������
				if (token_str == "int" || token_str == "char" || token_str == "float") {
					have_type = true;
					if (token_str == "int")
						type_type = 1;
					if (token_str == "char")
						type_type = 2;
					if (token_str == "float")
						type_type = 3;
				}


				//������� ��� � ������� ���������������
				if (token_str == "ID" && have_type && cur_row == 47) {
					identifier.set_ind_type(get_token_text(cur_t), type_type);
				}

				//���� ����� ���������� ��������� ���-�� ���������
				if (cur_row == 30 && cur_t.table_n != 5) {
					parse_error_f << "������ � ��������� " << get_token_text(cur_t) << " ��������� �� ����� ���� ���������� ��������" << endl;
					cout << "Lex error" << endl;
					local_error = true;
				}

				//� ����� ������
				cur_t = next_t;
				if (!parse_token_f.eof())
					parse_token_f >> next_t; //���� ���������, �� ��������� �����
			}

			if (parsing_table[cur_row].should_return) {
				prev_row = cur_row; //���������� ����������
				cur_row = parsing_stack.top(); //���� ���� ����� �� ����� - ����
				parsing_stack.pop();
				change_row = true;
			}


			if (!change_row && parsing_table[cur_row].jump != -1) {
				cur_row = parsing_table[cur_row].jump; //���� ���� �������� - �������
			}

		}
		else { //���� ���������� �������������
			if (parsing_table[cur_row].error) { //���� ����� ������ ��� ��� ������, �� ���������� �
				local_error = true;
				parse_error_f << "������ � ��������� " << get_token_text(cur_t) << endl;
				cout << "Lex error" << endl;

				//��� ��� 1, ������ - ����� �����������
				parse_error_f << "�������� �� ���� ����� ������ ����: ";
				do {
					for (int i = 0; i < parsing_table[cur_row].termenal.size(); i++) {
						parse_error_f << parsing_table[cur_row].termenal[i] << " ";
					}
					cur_row--;
				} while (!parsing_table[cur_row].error);
				parse_error_f << endl;
				//��� ��� 1 - �����
			}
			else { //���� ��� - ��������� �� ���������
				cur_row++;
			}
		}
		token_str_prev = token_str;
	};

	//��� ��� 2, ������ - ����������� �������� ����������� �����
	for (int i = 10; i < 32; i++) {
		bool is_els = true; //������ �� ��������
		int j = 0; //����� �������� � �������
		lexeme check_lex; //���������� �������
		while (is_els) {
			is_els = identifier.get_lexeme(i, j, check_lex);
			if (is_els) {
				if (check_lex.type == 0) {
					local_error = true;
					parse_error_f << "������ � ��������� �������������� " << check_lex.name << " �� �������� ���" << endl;
					cout << "Type error" << endl;
				}
				j++;
			}
		};
	}

	//��� ��� 2 - �����

	parse_token_f.close();
	parse_error_f.close();

	return !local_error;
}

//  ������������� ���� smallTreeCode � ���� ������,
//  ��� ����� ������� ���������� ���������. (";" or ",");
//
void translator::grow_tree(vector<token> code) {

	tree_el* little_tree_beg = new tree_el; //������ ���������� ������

	if (code.size() > 2) {
		grow_little_tree(code, little_tree_beg); //���������� ��������� �������

		//���� ��� ����� ������ ������
		if (tree_begin == 0) {
			tree_begin = little_tree_beg;
			tree_cur = tree_begin;
		}
		else {
			tree_cur->right = little_tree_beg; //� ������������ ��� � ��������
		}

		while (tree_cur->right != 0) { //���������� ������ ������
			tree_cur = tree_cur->right;
		};

	}

}

void translator::grow_little_tree(vector<token> code, tree_el *&beg) {

	vector<token> L, R; //����� � ������ ����������

	int bracket_num = 0; //���������� ������
	int L_num = 0;

	bool flag = false;


	//���� ��������� �����
	if (code.size() == 1 && (get_token_text(code[0]) == ";" || get_token_text(code[0]) == ",")) {
		beg->id = ";";
		beg->type = 2;
	}
	else {

		vector<int> opers_n; //������ "������� ��������"

		//������� ��� "������� ��������"
		for (int i = 0; i < code.size(); i++) {
			if (get_token_text(code[i]) == "(") bracket_num++;
			if (get_token_text(code[i]) == ")") bracket_num--;
			if (code[i].table_n == 2 && bracket_num == 0) opers_n.push_back(i);
		}

		if (opers_n.size() == 0) { //��� �������� ��� �������� �� ��������� � ����
			flag = false;
		}
		else { //���� ���� - ���� �������� � ���������� �����������
			int low_num = -1; //����� ������ ��� ��������
			//������� �������� �� ������� ���������
			for (int j = 0; j < opers_n.size() && low_num == -1; j++)
				if (get_token_text(code[opers_n[j]]) == "=" || get_token_text(code[opers_n[j]]) == "+=" || get_token_text(code[opers_n[j]]) == "-=" || get_token_text(code[opers_n[j]]) == "*=")
					low_num = opers_n[j];
			//����� �� + � -
			for (int j = opers_n.size() - 1; j >= 0 && low_num == -1; j--) {
				if (get_token_text(code[opers_n[j]]) == "+" || get_token_text(code[opers_n[j]]) == "-") {
					low_num = opers_n[j]; //����� ���� ��������
				}
			}

			//���� �� ����� �������� ��������� ��������
			if (low_num == -1)
				low_num = opers_n[opers_n.size() - 1];

			//������ �������������

			if (get_token_text(code[0]) == "(") L_num++;

			//��������� ����� ���������
			for (int j = 0; L_num < low_num; L_num++, j++)
				L.push_back(code[L_num]);

			//��������� ������ ���������
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

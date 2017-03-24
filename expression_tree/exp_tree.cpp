// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "expression_tree.h"
//bool is_oper(const string& s);
double get_number(const string& s, int& i);
int main()
{
//	int i = 0;
	//double a = get_number("123",i);
	//cout << a << " " << i << endl;
	exp_tree a,b;
	map<string, double> var();
	string s = "x/(2*x+1)";
	build_tree(s,a);
	//a.t_show();
	a.simplify();
	a.show();
	a.diff("x");
	a.show();
	a.simplify();
	a.show();
	//cout << "b: "; b.show();
	//a.set_var();
	//a.show();
	//cout << s<<"=" << a.eval() << endl;
	system("pause");
	return 0;
}
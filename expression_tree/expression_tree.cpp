// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// expression_tree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vld.h>
#include "expression_tree.h"
const double eps = 1e-6;
int compare(double a, double b)
{
	if (fabs(a - b) > eps)
	{
		if (a > b)
			return 1;
		else
			return -1;
	}
	return 0;
}
template <typename T>
bool in(const T& a, const set<T>& s)
{
	return s.find(a) != s.end();
}
double get_number(const string& s,int& i)
{
	int st = i,len=s.length();
	while (i<len && (isdigit(s[i]) || s[i] == '.'))
		i++;
	return atoi(s.substr(st,i-st).c_str());
}
string get_word(const string& s, int& i)
{
	int st = i, len = s.length();
	while (i < len && isalpha(s[i]))
		++i;
	return s.substr(st, i - st);
}
bool is_oper(const char& s)
{
	set<char> op = { '+','-','*','/','^' };
	return op.find(s)!=op.end();
}
int priority(const char& s)
{
	set<char> pr3 = { '^' };
	set<char> pr2 = { '*','/' };
	set<char> pr1 = { '+','-' };
	if (pr3.find(s) != pr3.end())
		return 3;
	if (pr2.find(s) != pr2.end())
		return 2;
	if (pr1.find(s) != pr1.end())
		return 1;
	return -1;
}
void delete_tree(Node * r)
{
	if (r)
	{
		delete_tree(r->left);
		delete_tree(r->right);
		delete r;
	}
}
Node * copy_tree(Node * rt)
{
	if (rt)
	{
		Node* temp = new Node(copy_tree(rt->left), copy_tree(rt->right), rt->type, rt->data, rt->val);
		return temp;
	}
	return nullptr;
}
Node * _diff(Node * rt,const string& var_n)
{
	if (!rt)
		return nullptr;
	if (!rt->right && !rt->left)
	{
		Node* temp;
		if (rt->type==Node::var && rt->data==var_n)
			temp = new Node(nullptr, nullptr, Node::constant, "",1);
		else
			temp= new Node(nullptr, nullptr, Node::constant, "", 0);
		return temp;
	}	
	if (in(rt->data, { "+","-" }))
	{
		Node* temp = new Node(_diff(rt->left,var_n), _diff(rt->right,var_n),rt->type, rt->data);
		return temp;
	}
	if (rt->data == "*")
	{
		Node* t1 = new_oper_node(
			new_oper_node(_diff(rt->left, var_n), copy_tree(rt->right), "*"),
			new_oper_node(copy_tree(rt->left), _diff(rt->right, var_n), "*"),
			"+");
		return t1;
	}
	if (rt->data == "/")
	{
		Node *t1 = new_oper_node(
			new_oper_node(
				new_oper_node(_diff(rt->left,var_n),copy_tree(rt->right),"*"), 
				new_oper_node(copy_tree(rt->left),_diff(rt->right,var_n),"*"),"-"),
			new_oper_node(copy_tree(rt->right),new_const_node(2),"^"),
			"/");
		return t1;
	}
	if (rt->data == "^")
	{
		if (rt->right->type == Node::constant)
		{

			Node *t1 = new Node(copy_tree(rt->left),
				new_const_node(rt->right->val-1),
				Node::oper, "^");
			Node* t2 = new Node(
				new_const_node(rt->right->val),
				t1,Node::oper,"*");
			Node *t3 = new Node(t2, _diff(rt->left, var_n), Node::oper, "*");
			return t3;
		}
	}
	return nullptr;
}
Node * new_node(Node * l, Node * r, int t, const string & d, double v)
{
	Node *temp = new Node(l, r, t, d, v);
	return temp;
}

void exp_tree::show_tree(Node * r) const
{
	if (r)
	{
		if (r->type == Node::constant)
			cout << r->val << " ";
		else
			cout << r->data << " ";
		show_tree(r->left);
		show_tree(r->right);
	}

}

bool exp_tree::_show(Node * r,string& s) const
{
	if (r == nullptr)
		return false;
	if (r->type == Node::oper)
	{
		bool s_par = in(r->data, { "*","/","^" });
		string s1, s2;
		bool s_l = _show(r->left, s1) && s_par, s_r = _show(r->right, s2) && s_par;
		s.clear();
		if (s_l) s='(';
		s += s1;
		if (s_l) s+= ')';
		s += r->data;
		if (s_r) s+='(';
		s += s2;
		if (s_r) s+= ')';
		return true;
	}
	else if (r->type == Node::constant)
	{
		char * buf = new char[100];
		_itoa(r->val,buf, 10);
		s = buf;
		delete[]buf;
	}
	else
		s = r->data;
	return false;
}
double calculate(double a, double b, const string& op)
{
	if (op == "+")
		return a + b;
	else if (op == "-")
		return a - b;
	else if (op == "*")
		return a*b;
	else if (op == "^")
		return pow(a, b);
	else if (op == "/" && compare(b,0))
		return a / b;
	else
		return 0;
}

void move_node(Node* r,int t,string d,double v)
{
	r->type = t;
	r->data = d;
	r->val = v;
	delete_tree(r->left);
	delete_tree(r->right);
	r->left = nullptr;
	r->right = nullptr;
}

void move_node_without_del(Node* rt, Node* l, Node* r, int t, string d, double v)
{
	rt->type = t;
	rt->data = d;
	rt->val = v;
	rt->left = l;
	rt->right = r;

}
void exp_tree::simplify(Node * r)
{
	if (r == nullptr || r->left == nullptr || r->right == nullptr)
		return;
	simplify(r->left);
	simplify(r->right);
	int r_t = r->right->type, l_t = r->left->type;
	if (l_t == Node::constant && r_t == Node::constant)
	{
		double a = r->left->val, b = r->right->val, res = calculate(a, b, r->data);
		move_node(r, Node::constant, "", res);
	}
	else if (l_t == Node::constant)
	{
		if (!compare(r->left->val, 1))
		{
			if (r->data == "*")
			{
				string t = r->right->data;
				Node *r_n = r->right;
				delete r->left;
				move_node_without_del(r,r_n->left,r_n->right, r_n->type, t, 0);
				delete r_n;
			}
		}
		else if (!compare(r->left->val, 0))
		{
			if (in(r->data, { "*","/" }))
				move_node(r, Node::constant, "", 0);
			else if (r->data == "+")
			{
				string t = r->right->data;
				Node *r_n = r->right;
				delete r->left;
				move_node_without_del(r, r_n->left, r_n->right, r_n->type, t, 0);
				delete r_n;
			}
		}
	}
	else if (r_t == Node::constant)
	{
		if (!compare(r->right->val, 1))
		{
			if (in(r->data, { "*","/" }))
			{
				string t = r->left->data;
				Node *l_n = r->left;
				delete r->right;
				move_node_without_del(r, l_n->left, l_n->right, l_n->type, t, 0);
				delete l_n;
			}
		}
		else if (!compare(r->right->val, 0))
		{
			if (r->data == "*")
				move_node(r, Node::constant, "", 0);
			else if (in(r->data, { "+","-" }))
			{
				string t = r->left->data;
				Node *l_n = r->left;
				delete r->right;
				move_node_without_del(r, l_n->left, l_n->right, l_n->type, t, 0);
				delete l_n;
			}
		}
	}
}
double exp_tree::eval(Node * r) const
{
	if (r->type == Node::oper)
	{
		string op = r->data;
		double a = eval(r->left), b = eval(r->right);
		return calculate(a, b, op);
	}
	else if (r->type == Node::constant)
		return r->val;
	else if (r->type == Node::var)
	{
		return 0;
		//return var.at(r->data);
	}
	return 0;
}
void calc_single_oper(stack<string>& op, stack<Node*>& tr)
{
	string cur = op.top();
	op.pop();
	Node *l, *r;
	r = tr.top(); tr.pop();
	l = tr.top(); tr.pop();
	Node *temp = new Node(l, r,Node::oper, cur);

	tr.push(temp);
}
void build_tree(const string & s,exp_tree& res)
{
	stack<string> oper;
	stack<Node*> tree;
	int len = s.length(),i=0;
	Node *temp;
	while (i<len)
	{
		if (s[i] == ')')
		{
			while (!oper.empty() && oper.top() != "(")
				calc_single_oper(oper, tree);
			oper.pop();
			++i;
		}
		else if (s[i] == '(')
			oper.push(string(1, s[i++]));
		else if (is_oper(s[i]))
		{
			int pr = priority(s[i]);
			while (!oper.empty() && pr < priority(oper.top()[0]))
				calc_single_oper(oper, tree);
			oper.push(string(1, s[i++]));
		}
		else if (isdigit(s[i]))
		{
			double d = get_number(s, i);
			temp = new Node(nullptr, nullptr, Node::constant, "", d);
			tree.push(temp);
		}
		else if (isalpha(s[i]))
		{
			string word = get_word(s, i);
			if (i >= len || s[i] == ')' || is_oper(s[i]))
			{
				temp = new Node(nullptr, nullptr, Node::var, word);
			//	res.var.insert({ word,0 });
				tree.push(temp);
			}
		}
		else
			++i;
	}
	while (!oper.empty())
		calc_single_oper(oper, tree);
	res.root = tree.top();
}

void exp_tree::t_show() const
{
	show_tree(root);
	cout << endl;
}

void exp_tree::set_var()
{
	/*for (auto it = var.begin(); it != var.end(); it++)
	{
		cout << it->first << "=";
		cin >> it->second;
	}*/
}

void exp_tree::show() const
{
	string s;
	_show(root,s);
	cout << s<<endl;
}

exp_tree::exp_tree(const exp_tree & a)
{
	root = copy_tree(a.root);
//	var = a.var;
}

exp_tree& exp_tree::operator=(const exp_tree & a)
{
	if (&a == this)
		return *this;
	this->~exp_tree();
	root = copy_tree(a.root);
	//var = a.var;
	return *this;
}

void exp_tree::diff(const string& var_n) 
{
	Node *rt = _diff(root,var_n);
	this->~exp_tree();
	root = rt;
}

exp_tree::~exp_tree()
{
	delete_tree(root);
}

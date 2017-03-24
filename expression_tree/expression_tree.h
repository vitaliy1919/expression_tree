#pragma once
#include "stdafx.h"
struct Node
{
	Node* left;
	Node* right;
	int type;
	enum {var,constant,oper};
	string data;
	double val;

	Node(Node* l=nullptr,Node* r=nullptr,int t=0,const string& d="",double v=0) :right(r),left(l),type(t),data(d),val(v) {}
};


void delete_tree(Node * r);
Node* copy_tree(Node* rt);
Node* _diff(Node* rt,const string& var_n);
Node* new_node(Node* l = nullptr, Node* r = nullptr, int t = 0, const string& d = "", double v = 0);
inline Node* new_oper_node(Node* l, Node* r, const string& d) {return new_node(l, r, Node::oper, d);}
inline Node *new_const_node(double v){ return new_node(nullptr, nullptr, Node::constant, "", v); }
class exp_tree
{
private:
	Node* root;
	map<string, double> var;
private:
	void show_tree(Node* r) const;
	bool _show(Node* r,string& s) const;
	void simplify(Node* r);
	double eval(Node* r) const;
	friend void move_node(Node* r, int t, string d, double v);

public:
	exp_tree(Node* r = nullptr) :root(r),var() {}
	friend void build_tree(const string& s,exp_tree& res);
	void t_show() const;
	void set_var();
	void simplify() { simplify(root); }
	void show() const;
	exp_tree(const exp_tree& a);
	exp_tree& operator=(const exp_tree& a);
	void diff(const string& var_n);
	double eval() const { return eval(root); }
	~exp_tree();
};
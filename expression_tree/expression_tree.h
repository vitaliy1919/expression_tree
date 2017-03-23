#pragma once
#include "stdafx.h"
struct Node
{
	Node* left;
	Node* right;
	Node* parent;
	int type;
	enum {var,constant,oper};
	string data;
	double val;

	Node(Node* l=nullptr,Node* r=nullptr,Node* p=nullptr,int t=0,const string& d="",double v=0) :right(r),left(l),parent(p),type(t),data(d),val(v) {}
};
void delete_tree(Node * r);
class exp_tree
{
	Node* root;
	map<string, double> var;
private:
	void show_tree(Node* r) const;
	bool _show(Node* r,string& s) const;
	void simplify(Node* r);
	double eval(Node* r) const;
	friend void move_node(Node* r, int t, string d, double v);
public:
	exp_tree(Node* r = nullptr) :root(r) {}
	friend void build_tree(const string& s,exp_tree& res);
	void t_show() const;
	void set_var();
	void simplify() { simplify(root); }
	void show() const;
	double eval() const { return eval(root); }
	~exp_tree();
};
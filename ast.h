/* Abstract Syntax Tree */
pragma once

#include <list>

namespace AST {

class Node;

typedef std::list<Node*> NodeList;

class Node { //nodo generalizado
public:
    virtual ~Node() {}
    virtual std::string printTree() {return "";}
    Type type;
};

class Block : public Node {
public:
	NodeList nodes;
	Block() {}
	std::string printTree();
};

class UnOp : public Node {
public:
	UnOperation op;
	Node* next;
	UnOp(UnOperation op, Node* next) : op(op), next(next) {
		this->type = next->type;
	}
	std::string printTree();
};

class BinOp : public Node {
public:
	BinOperation op;
	Node* left;
	Node* right;
	BinOp(Node* left, BinOperation op, Node* right) : left(left), op(op), right(right) { }
	std::string printTree();
};

class Variable : public Node {
public:
	std::string name;
	Node* next;
	Variable(std::string name, Node* next) : name(name), next(next) { }
	std::string printTree();
};

class Const : public Node { //nodo utilizado no uso de valores constantes (1, .1 ou até TRUE)
public:
	std::string value;
	Const(std::string value, Type type) : value(value), type(type) { }
	std::string printTree();
};

class AssignVar : public Node { //nodo utilizado na atribuição de variáveis. separado das operações binárias para facilitar a operação printTree()
public:
	Node* var;
	Node* value;
	AssignVar(Node* var, Node* value) : var(var), value(value), arrExpr(arrExpr) { }
	std::string printTree();
};

class DeclVar : public Node { //nodo utilizado na declaração de variáveis. separado das operações unárias para facilitar o printTree()
public:
	Node* next;
	DeclVar(Node* next) : next(next) {}
	std::string printTree();
};

class Par : public Node { //nodo utilizado quando há parênteses nas fórmulas
public:
	Node* content;
	Par(Node* content) : content(content) {
		this->type = content->type;
	}
	std::string printTree();
};

class FunCall : public Node {
public:
	std::string name;
	Node* args;
	FunCall(std::string name, Node* args) : name(name), args(args) {}
	std::string printTree();
};

class Return : public Node {
public:
	Node* expr;
	Return(Node* expr) : expr(expr) {}
	std::string printTree();
};

class Conditional : public Node {
public:
	Node* condition;
	Node* then;
	Node* _else;
	Conditional(Node* condition, Node* then, Node* _else) : condition(condition), then(then), _else(_else) {}
	std::string printTree();
};

class Loop : public Node {
public:
	Node* condition;
	Node* loopBlock;
	Loop(Node* condition, Node* loopBlock) : condition(condition), loopBlock(loopBlock) {}
	std::string printTree();
};

}
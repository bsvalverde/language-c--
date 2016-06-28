/* Abstract Syntax Tree */
#pragma once

#include <list>
#include <string>

#include "enums.h"

namespace AST {

class Node;

typedef std::list<Node*> NodeList;

class Node { //nodo generalizado
public:
    virtual ~Node() {}
    virtual std::string analyzeTree() { return ""; }
    Type type;
};

class Block : public Node {
public:
	NodeList nodes;
	Block() { }
	std::string analyzeTree();
};

class UnOp : public Node {
public:
	UnOperation op;
	Node* next;
	UnOp(UnOperation op, Node* next) : op(op), next(next) { this->type = next->type; }
	std::string analyzeTree();
};

class BinOp : public Node {
public:
	BinOperation op;
	Node* left;
	Node* right;
	BinOp(Node* left, BinOperation op, Node* right) : left(left), op(op), right(right) {
		this->type = Type::desconhecido;
	}
	std::string analyzeTree();
};

class Variable : public Node {
public:
	std::string name;
	Node* next;
	Variable(std::string name, Node* next, Type type) : name(name), next(next) { this->type = type; }
	std::string analyzeTree();
};

class Const : public Node { //nodo utilizado no uso de valores constantes (1, .1 ou até TRUE)
public:
	std::string value;
	Const(std::string value, Type type) : value(value) { this->type = type; }
	std::string analyzeTree();
};

class AssignVar : public Node { //nodo utilizado na atribuição de variáveis. separado das operações binárias para facilitar a operação analyzeTree()
public:
	Node* var;
	Node* value;
	AssignVar(Node* var, Node* value) : var(var), value(value) { }
	std::string analyzeTree();
};

class DeclVar : public Node { //nodo utilizado na declaração de variáveis. separado das operações unárias para facilitar o analyzeTree()
public:
	Node* next;
	DeclVar(Node* next) : next(next) { }
	std::string analyzeTree();
};

class Par : public Node { //nodo utilizado quando há parênteses nas fórmulas
public:
	Node* content;
	Par(Node* content) : content(content) { this->type = content->type; }
	std::string analyzeTree();
};

class Function : public Node {
public:
	std::string name;
	Node* params;
	Block* code;
	Function(std::string name, Node* params, Block* code, Type type) : name(name), params(params), code(code) { this->type = type; }
	std::string analyzeTree();
};

class Parameter : public Node {
public:
	std::string name;
	Node* next;
	Parameter(std::string name, Node* next, Type type) : name(name), next(next) { this->type = type; }
	std::string analyzeTree();
};

class FunCall : public Node {
public:
	std::string name;
	Node* args;
	FunCall(std::string name, Node* args) : name(name), args(args) { this->type = Type::desconhecido; }
	std::string analyzeTree();
};

class Arguments : public Node {
public:
	NodeList arguments;
	Arguments() { }
	std::string analyzeTree();
};

class Return : public Node {
public:
	Node* expr;
	Return(Node* expr) : expr(expr) { this->type = expr->type; }
	std::string analyzeTree();
};

class Conditional : public Node {
public:
	Node* condition;
	Node* then;
	Node* _else;
	Conditional(Node* condition, Node* then, Node* _else) : condition(condition), then(then), _else(_else) { }
	std::string analyzeTree();
};

class Loop : public Node {
public:
	Node* condition;
	Node* loopBlock;
	Loop(Node* condition, Node* loopBlock) : condition(condition), loopBlock(loopBlock) { }
	std::string analyzeTree();
};

}
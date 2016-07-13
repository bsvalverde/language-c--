/* Abstract Syntax Tree */
#pragma once

#include <list>
#include <stdlib.h> 
#include <string>

#include "llvm/IR/BasicBlock.h"

#include "enums.h"
#include "llvmbuilder.h"
#include "symtable.h"

namespace AST {

class Node;

typedef std::list<Node*> NodeList;

class Node { //nodo generalizado
public:
    virtual ~Node() {}
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder) { return nullptr; }
    virtual bool hasReturn() { return false; }
    Type type;
};

class Block : public Node {
public:
	NodeList nodes;
	Block() { }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class UnOp : public Node {
public:
	UnOperation op;
	Node* next;
	UnOp(UnOperation op, Node* next) : op(op), next(next) { this->type = next->type; }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class BinOp : public Node {
public:
	BinOperation op;
	Node* left;
	Node* right;
	BinOp(Node* left, BinOperation op, Node* right) : left(left), op(op), right(right) {
		switch(op){
			case plus: case sub: case mult: case _div:
				this->type = Type::_int;
				if(left->type == Type::_double || right->type == Type::_double)
					this->type = Type::_double;
				break;
				//TODO criar nodos coerção
				//TODO verificar se algum dos operandos é void
			case gt: case lt: case gte: case lte: case eq: case neq: case _and: case _or:
				this->type = Type::_bool;
				break;
		}
	}
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Variable : public Node {
public:
	std::string name;
	Node* next;
	ST::SymTable* parentST;
	Variable(std::string name, Node* next, Type type, ST::SymTable* parentST) : name(name), next(next), parentST(parentST) { this->type = type; }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Const : public Node { //nodo utilizado no uso de valores constantes (1, .1 ou até TRUE)
public:
	std::string value;
	Const(std::string value, Type type) : value(value) { this->type = type; }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class AssignVar : public Node { //nodo utilizado na atribuição de variáveis. separado das operações binárias para facilitar a operação analyzeTree()
public:
	Node* var;
	Node* value;
	ST::SymTable* parentST;
	AssignVar(Node* var, Node* value, ST::SymTable* parentST) : var(var), value(value), parentST(parentST)  { }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class DeclVar : public Node { //nodo utilizado na declaração de variáveis. separado das operações unárias para facilitar o analyzeTree()
public:
	Node* next;
	ST::SymTable* parentST;
	DeclVar(Node* next, ST::SymTable* parentST) : next(next), parentST(parentST)  { }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Par : public Node { //nodo utilizado quando há parênteses nas fórmulas
public:
	Node* content;
	Par(Node* content) : content(content) { this->type = content->type; }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Function : public Node {
public:
	std::string name;
	Node* params;
	Block* code;

	llvm::BasicBlock* functionBB;
	ST::SymTable* parentST;

	Function(std::string name, Node* params, Block* code, Type type, ST::SymTable* parentST) : name(name), params(params), code(code), parentST(parentST) {
		this->type = type;
		if(!this->hasReturn()){
			yyerror("semântico: função %s sem comando de retorno certamente alcançável.", this->name.c_str());
			generateCode = false;
		}
	}
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Parameter : public Node {
public:
	std::string name;
	Node* next;	
	Parameter(std::string name, Node* next, Type type) : name(name), next(next) { this->type = type; }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class FunCall : public Node {
public:
	std::string name;
	Node* args;
	ST::SymTable* parentST;
	FunCall(std::string name, Node* args, Type type, ST::SymTable* parentST) : name(name), args(args), parentST(parentST) { this->type = type; }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Arguments : public Node {
public:
	NodeList arguments;
	Arguments() { }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Return : public Node {
public:
	Node* expr;
	Return(Node* expr, Type type, Type fType) : expr(expr) {
		this->type = type;
		if(this->type != fType){
			if(this->type == Type::_void){
				yyerror("semântico: comando de retorno sem valor em função não void.");
				generateCode = false;
			} else {
				yyerror("semântico: comando de retorno com valor em função void.");
				generateCode = false;
			}
		}
	}
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Conditional : public Node {
public:
	Node* condition;
	Node* then;
	Node* _else;
	Conditional(Node* condition, Node* then, Node* _else) : condition(condition), then(then), _else(_else) { }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Loop : public Node {
public:
	Node* condition;
	Node* loopBlock;
	Loop(Node* condition, Node* loopBlock) : condition(condition), loopBlock(loopBlock) { }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
    //colocar um bool pra ver se é do while ou só while?
};

}
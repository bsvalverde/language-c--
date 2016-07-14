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

class Coertion : public Node {
public:
	Node* target;
	Coertion(Node* target, Type type) : target(target) { this->type = type; }
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
				if(left->type == Type::_void || right->type == Type::_void){
					yyerror("semântico: valor do tipo void utilizado em operação binária.");
				} else {
					if(left->type != this->type){
						this->left = new Coertion(left, this->type);
					}
					if(right->type != this->type){
						this->right = new Coertion(right, this->type);
					}
				}
				break;
			case gt: case lt: case gte: case lte: case eq: case neq: {
				this->type = Type::_bool;
				Type currentType = Type::_int;
				if(left->type == Type::_double || right->type == Type::_double)
					currentType = Type::_double;
				if(left->type == Type::_void || right->type == Type::_void){
					yyerror("semântico: valor do tipo void utilizado em operação binária.");
				} else {
					if(left->type != currentType)
						this->left = new Coertion(left, currentType);
					if(right->type != currentType)
						this->right = new Coertion(right, currentType);
				}
				break;
			}
			case _and: case _or:
				this->type = Type::_bool;
				if(left->type == Type::_void || right->type == Type::_void){
					yyerror("semântico: valor do tipo void utilizado em operação binária.");
				} else {
					if(left->type != this->type)
						this->left = new Coertion(left, this->type);
					if(right->type != this->type)
						this->right = new Coertion(right, this->type);
				}
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
	AssignVar(Node* var, Node* value, ST::SymTable* parentST) : var(var), value(value), parentST(parentST)  {
		if(value->type == Type::_void){
			yyerror("semântico: valor do tipo void não foi ignorado como deveria.");
		} else if(var->type != value->type){
			this->value = new Coertion(value, var->type);
		}
	}
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

class Arguments : public Node {
public:
	NodeList arguments;
	Arguments() { }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class FunCall : public Node {
public:
	std::string name;
	Node* args;
	ST::SymTable* parentST;
	FunCall(std::string name, Node* args, Type type, ST::SymTable* parentST) : name(name), args(args), parentST(parentST) {
		this->type = type;
		for(Node* arg : ((Arguments*)args)->arguments){
			if(arg->type == Type::_void) {
				yyerror("semântico: uso inválido de expressão void.");
			} else if(arg->type != this->type) {
				arg = new Coertion(arg, this->type);
			}
		}
	}
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

class Return : public Node {
public:
	Node* expr;
	Return(Node* expr, Type type, Type fType) : expr(expr) {
		this->type = type;
		if(this->type == Type::_void && fType != Type::_void){
			yyerror("semântico: comando de retorno sem valor em função não void.");
		} else if (this->type != Type::_void && fType == Type::_void){
			yyerror("semântico: comando de retorno com valor em função void.");
		} else if(this->type != fType){
			this->expr = new Coertion(expr, this->type);
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
	bool _do;
	Loop(Node* condition, Node* loopBlock, bool _do) : condition(condition), loopBlock(loopBlock), _do (_do) { }
    virtual llvm::Value* analyzeTree(LlvmBuilder* llvmbuilder);
    bool hasReturn();
};

}
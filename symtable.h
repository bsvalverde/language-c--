/* Symbol Table */
#pragma once

#include <map>

#include "ast.h"
#include "enums.h"

extern void yyerror(const char* s, ...);

namespace ST {

class Symbol;

class SymTable {
public:
	SymTable(SymTable* superScope);

	Symbol* addVariable(std::string name, Type type);
	Symbol* getVariable(std::string name);

	Symbol* addFunction(std::string name, Type type, std::list<Symbol*> parameters);
	Symbol* getFunction(std::string name, int argNo);
//private:
	bool hasSymbol(std::string name);

	std::map<std::string, Symbol*> table;
	SymTable* superScope;
};

class Symbol {
public:
	Symbol(std::string name, Kind kind=Kind::undefined) { }
	Symbol(std::string name, Type type, Kind kind=Kind::undefined) : name(name), type(type) { }
	void setType(Type type);

	std::string name;
	Type type;
	Kind kind;

	llvm::AllocaInst* inst;
};

class Variable : public Symbol {
public:
	Variable(std::string name);

private:
	//AST::Node* lastModification;
};

class Function : public Symbol {
public:
	Function(std::string name, std::list<Symbol*> parameters);

//private:
	std::list<Symbol*> parameters;
	//AST::Block* code;
};

}
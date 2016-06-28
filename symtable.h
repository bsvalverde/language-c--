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
	Symbol* addSymbol(std::string name, Type type);
	Symbol* getSymbol(std::string name);
private:
	bool hasSymbol(std::string name);

	std::map<std::string, Symbol*> table;
	SymTable* superScope;
};

class Symbol {
public:
	Symbol(std::string name, SymbolType symbolType=SymbolType::undefined) { }
	Symbol(std::string name, Type type, SymbolType symbolType=SymbolType::undefined) : name(name), type(type) { }
	void setType(Type type);

	std::string name;
	Type type;
	//TYPE?! RENOMEAR TODO
	SymbolType symbolType;
};

class Variable : public Symbol {
public:
	Variable(std::string name);

private:
	AST::Node* lastModification;
};

class Function : public Symbol {
public:
	Function(std::string name);

private:
	//TODO ter uma lista de simbolos representando os parametros da funcao?
	AST::Block* code;
};

}
/* Symbol Table */
#pragma once

#include <map>
#include "enums.h"

extern void yyerror(const char* s, ...);

namespace ST {

class Symbol;

class SymTable {
public:
	SymTable();
	Symbol* addSymbol(std::string name);
	Symbol* addSymbol(std::string name, Type type);
	Symbol* getSymbol(std::string name);
	void setSymbol(std::string name);
private:
	bool hasSymbol(std::string name);

	std::map<std::string, Symbol*> table;
};

class Symbol {
public:
	Symbol();
	Symbol(std::string name, Type type) : name(name), type(type) {}
	void setType(Type type);

	std::string name;
	Type type;
};

}
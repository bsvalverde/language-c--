#include "symtable.h"

using namespace ST;

SymTable::SymTable(SymTable* superScope) {
	this->superScope = superScope;
}

Symbol* SymTable::addVariable(std::string name, Type type) {
	if(hasSymbol(name)) {
		Symbol* sym = this->table[name];
		if(sym->kind != Kind::variable){
			yyerror("semântico: variável %s previamente declarada como função.", name.c_str());
		} else if (sym->type != type){
			yyerror("semântico: variável %s previamente declarada com outro tipo.", name.c_str());
		} else {
			yyerror("semântico: redeclaração da variável %s.", name.c_str());
		}
		return new Symbol(name);
	}
	this->table[name] = new Variable(name);
	this->table[name]->type = type;
	return this->table[name];
}

Symbol* SymTable::getVariable(std::string name) {
	if(hasSymbol(name)) {
		Symbol* sym = this->table[name];
		if(sym->kind != Kind::variable) {
			yyerror("semântico: uso da função %s como variável.", name.c_str());
		}
		return sym;
	}
	if(this->superScope == NULL) {
		yyerror("semântico: variável %s não foi declarada neste escopo.", name.c_str());
		return new Symbol(name);
	} else {
		return this->superScope->getVariable(name);
	}
}

Symbol* SymTable::addFunction(std::string name, Type type, std::list<Symbol*> parameters) {
	if(hasSymbol(name)) {
		Symbol* sym = this->table[name];
		if(sym->kind != Kind::function){
			yyerror("semântico: função %s previamente declarada como variável.", name.c_str());
		} else {
			yyerror("semântico: redeclaração da função %s.", name.c_str());
		}
		return new Symbol(name);
	}
	this->table[name] = new Function(name, parameters);
	this->table[name]->type = type;
	return this->table[name];
}

Symbol* SymTable::getFunction(std::string name, int argNo) {
	if(hasSymbol(name)) {
		Symbol* sym = this->table[name];
		if(sym->kind != Kind::function) {
			yyerror("semântico: uso da variável %s como função.", name.c_str());
		} else if(argNo < ((Function*)sym)->parameters.size()){
			yyerror("semântico: função %s chamada com poucos argumentos.", name.c_str());
		} else if(argNo > ((Function*)sym)->parameters.size()){
			yyerror("semântico: função %s chamada com muitos argumentos.", name.c_str());
		}
		return sym;
	}
	if(this->superScope == NULL) {
		yyerror("semântico: função %s não foi declarada neste escopo.", name.c_str());
		return new Symbol(name);
	} else {
		return this->superScope->getFunction(name, argNo);
	}
}

bool SymTable::hasSymbol(std::string name) {
	return this->table.find(name) != this->table.end();
}

Variable::Variable(std::string name) : Symbol(name) {
	this->kind = Kind::variable;
}

Function::Function(std::string name, std::list<Symbol*> parameters) : Symbol(name), parameters(parameters) {
	this->kind = Kind::function;
}
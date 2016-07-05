#include "symtable.h"

using namespace ST;

SymTable::SymTable(SymTable* superScope) {
	this->superScope = superScope;
}

//TODO precisa retornar alguma coisa?
Symbol* SymTable::addSymbol(std::string name, Type type) {
	//TODO e se addSymbol entrar no if e não adicionar?
	if(hasSymbol(name)) {
		yyerror("ERROR: Variable %s previously declared.", name.c_str());
		//TODO o que retornar nesse caso?
		return new Symbol(name);
	}
	this->table[name] = new Symbol(name);
	this->table[name]->type = type;
	return this->table[name];
}

Symbol* SymTable::getSymbol(std::string name) {
	if(hasSymbol(name)) {
		return this->table[name];
	}
	if(this->superScope == NULL) {
		yyerror("ERROR: Unknown variable %s.", name.c_str());
		//TODO o que retornar nesse caso?
		return new Symbol(name);
	} else {
		return this->superScope->getSymbol(name);
	}
}

Symbol* SymTable::addVariable(std::string name, Type type) {
	if(hasSymbol(name)) {
		yyerror("ERROR: Variable %s previously declared.", name.c_str());
		return new Symbol(name);
	}
	this->table[name] = new Variable(name);
	this->table[name]->type = type;
	return this->table[name];
}

Symbol* SymTable::getVariable(std::string name) {
	if(hasSymbol(name)) {
		return this->table[name];
	}
	if(this->superScope == NULL) {
		yyerror("ERROR: Unknown variable %s.", name.c_str());
		return new Symbol(name);
	} else {
		return this->superScope->getSymbol(name);
	}
}

Symbol* SymTable::addFunction(std::string name, Type type) {
	if(hasSymbol(name)) {
		yyerror("ERROR: Variable %s previously declared.", name.c_str());
		return new Symbol(name);
	}
	this->table[name] = new Function(name);
	this->table[name]->type = type;
	return this->table[name];
}

Symbol* SymTable::getFunction(std::string name) {
	//TODO
	SymTable* global = this;
	while(global->superScope != NULL){
		global = global->superScope;
	}
	if(hasSymbol(name)) {
		return this->table[name];
	}
	yyerror("ERROR: Unknown variable %s.", name.c_str());
	return new Symbol(name);
}

bool SymTable::hasSymbol(std::string name) {
	return this->table.find(name) != this->table.end();
}

Variable::Variable(std::string name) : Symbol(name) {
	this->kind = Kind::variable;
}

Function::Function(std::string name) : Symbol(name) {
	this->kind = Kind::function;
}
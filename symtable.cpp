#include "symtable.h"

using namespace ST;

SymTable::SymTable(SymTable* superScope) {
	this->superScope = superScope;
}

//TODO precisa retornar alguma coisa?
Symbol* SymTable::addSymbol(std::string name) {
	if(hasSymbol(name)) {
		yyerror("ERROR: Variable %s previously declared.", name.c_str());
		//TODO o que retornar nesse caso?
		return new Symbol(name);
	} else {
		this->table[name] = new Symbol(name);
		return this->table[name];
	}
}

Symbol* SymTable::addSymbol(std::string name, Type type) {
	//TODO e se addSymbol entrar no if e não adicionar?
	this->addSymbol(name);
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

bool SymTable::hasSymbol(std::string name) {
	return this->table.find(name) != this->table.end();
}

Variable::Variable(std::string name) : Symbol(name) {
	this->symbolType = SymbolType::variable;
}

Function::Function(std::string name) : Symbol(name) {
	this->symbolType = SymbolType::function;
}
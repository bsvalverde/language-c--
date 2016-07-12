#include "symtable.h"

using namespace ST;

SymTable::SymTable(SymTable* superScope) {
	this->superScope = superScope;
}

Symbol* SymTable::addVariable(std::string name, Type type) {
	if(type == Type::_void){
		yyerror("semântico: variável %s declarada com tipo void.", name.c_str());
		generateCode = false;
		return new Symbol(name);
	}
	if(hasSymbol(name)) {
		Symbol* sym = this->table[name];
		if(sym->kind != Kind::variable){
			yyerror("semântico: variável %s previamente declarada como função.", name.c_str());
			generateCode = false;
		} else if (sym->type != type){
			yyerror("semântico: variável %s previamente declarada com outro tipo.", name.c_str());
			generateCode = false;
		} else {
			yyerror("semântico: redeclaração da variável %s.", name.c_str());
			generateCode = false;
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
			generateCode = false;
		}
		return sym;
	}
	if(this->superScope == NULL) {
		yyerror("semântico: variável %s não foi declarada neste escopo.", name.c_str());
		generateCode = false;
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
			generateCode = false;
		} else {
			yyerror("semântico: redeclaração da função %s.", name.c_str());
			generateCode = false;
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
			generateCode = false;
		} else if(argNo < ((Function*)sym)->parameters.size()){
			yyerror("semântico: função %s chamada com poucos argumentos.", name.c_str());
			generateCode = false;
		} else if(argNo > ((Function*)sym)->parameters.size()){
			yyerror("semântico: função %s chamada com muitos argumentos.", name.c_str());
			generateCode = false;
		}
		return sym;
	}
	if(this->superScope == NULL) {
		yyerror("semântico: função %s não foi declarada neste escopo.", name.c_str());
		generateCode = false;
		return new Symbol(name);
	} else {
		return this->superScope->getFunction(name, argNo);
	}
}

void SymTable::searchForMain(){
	if(!this->hasSymbol("main")){
		yyerror("semântico: função main() não existe.");
		generateCode = false;
	} else {
		Symbol* s = this->table["main"];
		if(s->kind != Kind::function){
			yyerror("semântico: função main() não existe.");
			generateCode = false;
		} else {
			Function* f = (Function*)s;
			if(f->parameters.size() > 0){
				yyerror("semântico: função main() não pode ter argumentos.");
				generateCode = false;
			}
		}
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
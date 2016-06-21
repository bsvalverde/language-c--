#include "symtable.h"

using namespace ST;

SymTable::SymTable(SymTable* superScope) {
	this->superScope = superScope;
}

Symbol* SymTable::addSymbol(std::string name) {
	return 0;
}

Symbol* SymTable::addSymbol(std::string name, Type type) {
	return 0;

}

Symbol* SymTable::getSymbol(std::string name) {
	return 0;

}

bool SymTable::hasSymbol(std::string name) {
	return 0;

}

Variable::Variable() : Symbol() {

}

Function::Function() : Symbol() {

}
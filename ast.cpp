#include "ast.h"

using namespace AST;

std::string Block::analyzeTree() {
	std::string retorno = "";
	for(Node* node : nodes) {
		if(node != NULL)
			retorno += node->analyzeTree() + "\n";
	}
	return retorno;
}

std::string UnOp::analyzeTree() {
	return "UnOp";
}

std::string BinOp::analyzeTree() {
	return "BinOp";
}

std::string Variable::analyzeTree() {
	return "Variable";
}

std::string Const::analyzeTree() {
	return "Const";
}

std::string AssignVar::analyzeTree() {
	return "AssignVar";
}

std::string DeclVar::analyzeTree() {
	return "DeclVar";
}

std::string Par::analyzeTree() {
	return "Par";
}

std::string Function::analyzeTree() {
	return "Function";
}

std::string Parameter::analyzeTree() {
	return "Parameter";
}

std::string FunCall::analyzeTree() {
	return "FunCall";
}

std::string Arguments::analyzeTree() {
	return "Arguments";
}

std::string Return::analyzeTree() {
	return "Return";
}

std::string Conditional::analyzeTree() {
	printf("entrando no cond");
	std::string retorno = "Expressao condicional";
	retorno += "\nIF:\n" + this->condition->analyzeTree();
	retorno += "\nTHEN:\n" + this->then->analyzeTree();
	if(this->_else != NULL){
		retorno += "ELSE:\n" + this->_else->analyzeTree();
	}
	return retorno;
}

std::string Loop::analyzeTree() {
	return "Loop";
}
// #include "ast.h"

// using namespace AST;

// std::string Block::analyzeTree() {
// 	std::string retorno = "";
// 	for(Node* node : nodes) {
// 		if(node != NULL)
// 			retorno += node->analyzeTree() + "\n";
// 	}
// 	return retorno;
// }

// std::string UnOp::analyzeTree() {
// 	return "UnOp";
// }

// std::string BinOp::analyzeTree() {
// 	return "BinOp";
// }

// std::string Variable::analyzeTree() {
// 	return "Variable";
// }

// std::string Const::analyzeTree() {
// 	return "Const";
// }

// std::string AssignVar::analyzeTree() {
// 	return "AssignVar";
// }

// std::string DeclVar::analyzeTree() {
// 	return "DeclVar";
// }

// std::string Par::analyzeTree() {
// 	return "Par";
// }

// std::string Function::analyzeTree() {
// 	return "Function";
// }

// std::string Parameter::analyzeTree() {
// 	return "Parameter";
// }

// std::string FunCall::analyzeTree() {
// 	return "FunCall";
// }

// std::string Arguments::analyzeTree() {
// 	return "Arguments";
// }

// std::string Return::analyzeTree() {
// 	return "Return";
// }

// std::string Conditional::analyzeTree() {
// 	printf("entrando no cond");
// 	std::string retorno = "Expressao condicional";
// 	retorno += "\nIF:\n" + this->condition->analyzeTree();
// 	retorno += "\nTHEN:\n" + this->then->analyzeTree();
// 	if(this->_else != NULL){
// 		retorno += "ELSE:\n" + this->_else->analyzeTree();
// 	}
// 	return retorno;
// }

// std::string Loop::analyzeTree() {
// 	return "Loop";
// }

#include "ast.h"
#include "stringfier.h"

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
	std::string retorno = "";
	retorno = next->analyzeTree() + ")";
	retorno = "((" + Stringfier::unOpString(op) + " " + Stringfier::typeStringM(next->type) + ") " + retorno;
	this->type = next->type;
	return retorno;
}

std::string BinOp::analyzeTree() {
	std::string retorno, lvalue, rvalue;
	retorno = "";
	lvalue = left->analyzeTree();
	rvalue = right->analyzeTree();
	std::string opString = Stringfier::binOpString(op);
	if (left->type != right->type){
		if(left->type == Type::_int && right->type == Type::_double){
			lvalue += " para real";
		} else if (left->type == Type::_double && right->type == Type::_int){
			rvalue += " para real";
		}
	}
    switch(op){
	case plus: case sub: case mult: case _div:
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringF(this->type) + ") " + rvalue + ")";
		break;
	case gt: case lt: case gte: case lte: case eq: case neq:
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case _and: case _or:
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
    }
    return retorno;
}

std::string Variable::analyzeTree() {
	std::string retorno = "variavel " + Stringfier::typeStringF(this->type) + " " + this->name;
	return retorno;
}

std::string Const::analyzeTree() {
	std::string retorno = "valor " + Stringfier::typeStringM(this->type) + " " + this->value;
	return retorno;
}

std::string AssignVar::analyzeTree() {
	std::string retorno, lvalue, rvalue;
	retorno = "";
	lvalue = var->analyzeTree();
	rvalue = value->analyzeTree();
	if (var->type != value->type && var->type == Type::_double && value->type == Type::_int){
		rvalue += " para real";
	}
	return "Atribuicao de valor para " + lvalue + ": " + rvalue;;
}

std::string DeclVar::analyzeTree() {
	std::string retorno = "";
	
	Variable* next = (Variable *)this->next;
	retorno = "Declaracao de variavel " + Stringfier::typeStringF(next->type);
	retorno += ": ";
	
	std::string vars = next->name;
	next = (Variable *)next->next;
	while(next != NULL){
		vars = next->name + ", " + vars;
		next = (Variable *)next->next;
	}
	retorno += vars;
	return retorno;
}

std::string Par::analyzeTree() {
	std::string retorno = "((abre parenteses) " + this->content->analyzeTree() + " (fecha parenteses))";
	return retorno;
}

std::string Function::analyzeTree() {
	//TODO
	// FT::Function* fun = funtable->getFunction(this->funName);
	// std::string retorno = "Declaracao de funcao " + Stringfier::typeStringF(fun->returnType) + ": " + this->funName + "\n";

	// retorno += this->parameters->analyzeTree();
	// retorno += "Fim declaracao";
	// return retorno;
	return "";
}

std::string Parameter::analyzeTree(){
	std::string retorno = "parametro " + Stringfier::typeStringM(this->type) + " " + this->name;
	return retorno;
}

std::string FunCall::analyzeTree() {
	std::string retorno = "chamada de funcao " + Stringfier::typeStringF(this->type) + " " + this->name;
	retorno += " " + args->analyzeTree();
	return retorno;
}

std::string Arguments::analyzeTree(){
	std::string retorno = "{+parametros: ";
	for(Node* node : arguments) {
		if(node != NULL)
			retorno += ", " + node->analyzeTree() + "\n";
	}
	retorno += "}";
	return retorno;
}

std::string Return::analyzeTree() {
	return "Retorno de funcao: " + this->expr->analyzeTree();
}

std::string Conditional::analyzeTree() {
	std::string retorno = "Expressao condicional";
	retorno += "\n+se:\n" + this->condition->analyzeTree();
	retorno += "\n+entao:\n" + this->then->analyzeTree();
	if(this->_else != NULL){
		retorno += "+senao:\n" + this->_else->analyzeTree();
	}
	retorno += "Fim expressao condicional";
	return retorno;
}

std::string Loop::analyzeTree() {
	std::string retorno = "+enquanto: ";
	retorno += this->condition->analyzeTree();
	retorno += "\n+faca:\n";
	retorno += this->loopBlock->analyzeTree();
	retorno += "Fim laco";
	return retorno;
}
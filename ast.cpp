#include "ast.h"

using namespace AST;

// ST::SymTable* symtable = new ST::SymTable(NULL);

llvm::Value* Block::analyzeTree(LlvmBuilder* llvmbuilder) {
	for(Node* node : nodes) {
		if(node != NULL)
			node->analyzeTree(llvmbuilder);
	}

	return nullptr;
}

llvm::Value* UnOp::analyzeTree(LlvmBuilder* llvmbuilder) {
	return nullptr;
}

llvm::Value* BinOp::analyzeTree(LlvmBuilder* llvmbuilder) {
	llvm::Value* lvalue = left->analyzeTree(llvmbuilder);
	llvm::Value* rvalue = right->analyzeTree(llvmbuilder);

	switch(op) {
	case plus:
		return llvmbuilder->buildSumInt(lvalue, rvalue);
	case sub:
		return llvmbuilder->buildSubInt(lvalue, rvalue);
	case mult:
		return llvmbuilder->buildMulInt(lvalue, rvalue);
	case _div:
		return llvmbuilder->buildDivInt(lvalue, rvalue);
	case gt: case lt: case gte: case lte: case eq: case neq:
		return nullptr;
	case _and: 
		return llvmbuilder->buildAnd(lvalue, rvalue);
	case _or:
		return llvmbuilder->buildOr(lvalue, rvalue);
	}
}

llvm::Value* Variable::analyzeTree(LlvmBuilder* llvmbuilder) {
	llvm::AllocaInst* inst = this->parentST->getVariable(this->name)->inst;

	return llvmbuilder->loadVariable(this->name, inst);
}

llvm::Value* Const::analyzeTree(LlvmBuilder* llvmbuilder) {
	switch(type) {
	case _int:
		return llvmbuilder->buildInt(atoi(value.c_str()));
	case _double:
		return llvmbuilder->buildDouble(atof(value.c_str()));
	case _bool:
		return llvmbuilder->buildBool(value != "0");
	case _void: case desconhecido:
		return nullptr; // Nunca deve chegar aqui
	}
}

llvm::Value* AssignVar::analyzeTree(LlvmBuilder* llvmbuilder) {	
	std::string varName = ((Variable*) var)->name;

	llvm::AllocaInst* inst = this->parentST->getVariable(((Variable*) var)->name)->inst;
	llvm::Value* rvalue = value->analyzeTree(llvmbuilder);
	llvmbuilder->setVariable(rvalue, inst);		

	return nullptr;
}

llvm::Value* DeclVar::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::string varName = ((Variable*) next)->name;
	this->parentST->getVariable(((Variable*) next)->name)->inst = llvmbuilder->storeVariable(varName, nullptr);

	return nullptr;
}

llvm::Value* Par::analyzeTree(LlvmBuilder* llvmbuilder) {
	return nullptr;
}

llvm::Value* Function::analyzeTree(LlvmBuilder* llvmbuilder) {
	llvm::Function* function = llvmbuilder->createFunction(this->name);
	this->functionBB = llvmbuilder->createBasicBlock(function, this->name+"BB");
	llvmbuilder->setInsertPoint(this->functionBB);

	code->analyzeTree(llvmbuilder);

	return function;
}

llvm::Value* Parameter::analyzeTree(LlvmBuilder* llvmbuilder) {
	return nullptr;
}

llvm::Value* FunCall::analyzeTree(LlvmBuilder* llvmbuilder) {
	return nullptr;
}

llvm::Value* Arguments::analyzeTree(LlvmBuilder* llvmbuilder) {
	return nullptr;
}

llvm::Value* Return::analyzeTree(LlvmBuilder* llvmbuilder) {
	return nullptr;
}

llvm::Value* Conditional::analyzeTree(LlvmBuilder* llvmbuilder) {
	// printf("entrando no cond");
	// std::string retorno = "Expressao condicional";
	// retorno += "\nIF:\n" + this->condition->analyzeTree(llvmbuilder);
	// retorno += "\nTHEN:\n" + this->then->analyzeTree(llvmbuilder);
	// if(this->_else != NULL){
	// 	retorno += "ELSE:\n" + this->_else->analyzeTree(llvmbuilder);
	// }
	// return retorno;
	return nullptr;
}

llvm::Value* Loop::analyzeTree(LlvmBuilder* llvmbuilder) {
	return nullptr;
}
#include "ast.h"

using namespace AST;

// ST::SymTable* symtable = new ST::SymTable(NULL);

llvm::Value* Block::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Block" << std::endl;
	for(Node* node : nodes) {
		if(node != NULL)
			node->analyzeTree(llvmbuilder);
	}

	return nullptr;
}

llvm::Value* UnOp::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "UnOp" << std::endl;

	llvm::Value* value = next->analyzeTree(llvmbuilder);

	switch(op) {
	case _not:
		return llvmbuilder->buildNot(value);
	case neg:
		return llvmbuilder->buildNeg(value);
	}

	return nullptr;
}

llvm::Value* BinOp::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "BinOp" << std::endl;
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
	case gt: 
		return llvmbuilder->buildGtInt(lvalue, rvalue);
	case lt: 
		return llvmbuilder->buildLtInt(lvalue, rvalue);
	case gte: 
		return llvmbuilder->buildGeInt(lvalue, rvalue);
	case lte: 
		return llvmbuilder->buildLeInt(lvalue, rvalue);
	case eq: 
		return llvmbuilder->buildEqInt(lvalue, rvalue);
	case neq:
		return llvmbuilder->buildNeInt(lvalue, rvalue);
	case _and: 
		return llvmbuilder->buildAnd(lvalue, rvalue);
	case _or:
		return llvmbuilder->buildOr(lvalue, rvalue);
	}
}

llvm::Value* Variable::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Variable" << std::endl;
	llvm::AllocaInst* inst = this->parentST->getVariable(this->name)->inst;

	return llvmbuilder->loadVariable(this->name, inst);
}

llvm::Value* Const::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Const" << std::endl;
	switch(type) {
	case _int:
		return llvmbuilder->buildInt(atoi(value.c_str()));
	case _double:
		return llvmbuilder->buildDouble(atof(value.c_str()));
	case _bool:
		return llvmbuilder->buildBool(value == "true");
	case _void: case desconhecido:
		return nullptr; // Nunca deve chegar aqui
	}
}

llvm::Value* AssignVar::analyzeTree(LlvmBuilder* llvmbuilder) {	
	std::cout << "AssignVar" << std::endl;
	Variable* variable = (Variable*) var;
	std::string varName = variable->name;

	llvm::AllocaInst* inst = this->parentST->getVariable(variable->name)->inst;

	if(!inst) {
		this->parentST->getVariable(variable->name)->inst = llvmbuilder->storeVariable(varName, nullptr);
		inst = this->parentST->getVariable(variable->name)->inst;
	}

	llvm::Value* rvalue = value->analyzeTree(llvmbuilder);
	llvmbuilder->setVariable(rvalue, inst);		
	return nullptr;
}

llvm::Value* DeclVar::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "DeclVar" << std::endl;
	while(next) {
		Variable* var = ((Variable*) next);

		std::string varName = var->name;
		this->parentST->getVariable(var->name)->inst = llvmbuilder->storeVariable(varName, nullptr);	

		next = var->next;
	}

	return nullptr;
}

llvm::Value* Par::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Par" << std::endl;

	return this->content->analyzeTree(llvmbuilder);
}

llvm::Value* Function::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Function" << std::endl;
	llvm::Function* function = llvmbuilder->createFunction(this->name);
	this->functionBB = llvmbuilder->createBasicBlock(function, this->name+"BB");
	llvmbuilder->setInsertPoint(this->functionBB);

	code->analyzeTree(llvmbuilder);

	return function;
}

llvm::Value* Parameter::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Parameter" << std::endl;
	return nullptr;
}

llvm::Value* FunCall::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "FunCall" << std::endl;
	return nullptr;
}

llvm::Value* Arguments::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Arguments" << std::endl;
	return nullptr;
}

llvm::Value* Return::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Return" << std::endl;
	return nullptr;
}

llvm::Value* Conditional::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Conditional" << std::endl;

	llvm::Value* cond = this->condition->analyzeTree(llvmbuilder);

	llvm::BasicBlock* ip = llvmbuilder->getCurrentBasicBlock();

	llvm::BasicBlock* _if = llvmbuilder->createBasicBlock(ip->getParent(), "if");
	llvm::BasicBlock* _else = llvmbuilder->createBasicBlock(ip->getParent(), "else");

	llvmbuilder->setInsertPoint(_if);
	this->then->analyzeTree(llvmbuilder);

	if(this->_else) {
		llvmbuilder->setInsertPoint(_else);
		this->_else->analyzeTree(llvmbuilder);
	}

	llvmbuilder->setInsertPoint(ip);
	return llvmbuilder->createCondBranch(_if, cond, _else);

	// printf("entrando no cond");
	// std::string retorno = "Expressao condicional";
	// retorno += "\nIF:\n" + this->condition->analyzeTree(llvmbuilder);
	// retorno += "\nTHEN:\n" + this->then->analyzeTree(llvmbuilder);
	// if(this->_else != NULL){
	// 	retorno += "ELSE:\n" + this->_else->analyzeTree(llvmbuilder);
	// }
	// return retorno;
}

llvm::Value* Loop::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Loop" << std::endl;
	return nullptr;
}
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

// Function usa Parameters
llvm::Value* Function::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Function" << std::endl;
	llvm::Function* function = llvmbuilder->createFunction(this->name);

	Parameter* params = (Parameter*) this->params;
	int argSize = 0;
	if(params) {
		Variable* var = (Variable*) params->next;
		while(var) {
			var = (Variable*) var->next;
			argSize++;
		}	
	}

	this->parentST->getFunction(this->name, argSize)->func = function;

	this->functionBB = llvmbuilder->createBasicBlock(function, this->name+"BB");
	llvmbuilder->setInsertPoint(this->functionBB);

	code->analyzeTree(llvmbuilder);

	return function;
}

// TODO retornar ArrayRef de Value*, e não Value*
llvm::Value* Parameter::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Parameter" << std::endl;
	return nullptr;
}

// FunCall usa Arguments
llvm::Value* FunCall::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "FunCall" << std::endl;

	std::vector<llvm::Value*> argsArrayRef;
	Arguments* args = (Arguments*) this->args;

	Variable* var;
	for(Node* node : args->arguments) {
		if(node != NULL)
			var = (Variable*) node;
			
			llvm::AllocaInst* inst = this->parentST->getVariable(var->name)->inst;
			argsArrayRef.push_back(inst);
	}

	llvm::Function* func = ((ST::Function*) this->parentST->getFunction(this->name, args->arguments.size()))->func;

	return llvmbuilder->createFunctionCall(func, llvm::makeArrayRef(argsArrayRef));
	// return nullptr;
}

llvm::Value* Arguments::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Arguments" << std::endl;
	return nullptr;
}

llvm::Value* Return::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Return" << std::endl;
	llvm::Value* retVal = this->expr->analyzeTree(llvmbuilder);
	llvmbuilder->createReturn(retVal);
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
}

llvm::Value* Loop::analyzeTree(LlvmBuilder* llvmbuilder) {
	std::cout << "Loop" << std::endl;

	llvm::BasicBlock* beforeBB = llvmbuilder->getCurrentBasicBlock();

	llvm::BasicBlock* loopBB = llvmbuilder->createBasicBlock(beforeBB->getParent(), "loop");
	llvm::BasicBlock* afterBB = llvmbuilder->createBasicBlock(beforeBB->getParent(), "after");

	llvm::Value* cond;
	if(this->_do) {
		llvmbuilder->createBranch(loopBB);
	} else {
		cond = this->condition->analyzeTree(llvmbuilder);
		llvmbuilder->createCondBranch(loopBB, cond, afterBB);
	}

	llvmbuilder->setInsertPoint(loopBB);
	this->loopBlock->analyzeTree(llvmbuilder);

	cond = this->condition->analyzeTree(llvmbuilder);
	llvmbuilder->createCondBranch(loopBB, cond, afterBB);

	llvmbuilder->setInsertPoint(afterBB);

	return nullptr;
}

llvm::Value* Coertion::analyzeTree(LlvmBuilder* llvmbuilder){
	std::cout << "Coertion" << std::endl;
	return target->analyzeTree(llvmbuilder);
}

bool Block::hasReturn() {
	bool ret = false;
	for(Node* node : nodes) {
		if(node != NULL)
			if(node->hasReturn()){
				ret = true;
				break;
			}
	}
	return ret;
}

bool UnOp::hasReturn() {
	return false;
}

bool BinOp::hasReturn() {
	return false;
}

bool Variable::hasReturn() {
	return false;
}

bool Const::hasReturn() {
	yyerror("fatal: não devia chegar aqui!");
	return false;
}

bool AssignVar::hasReturn() {	
	return false;
}

bool DeclVar::hasReturn() {
	return false;
}

bool Par::hasReturn() {
	yyerror("fatal: não devia chegar aqui!");
	return false;
}

bool Function::hasReturn() {
	return code->hasReturn();
}

bool Parameter::hasReturn() {
	yyerror("fatal: não devia chegar aqui!");
	return false;
}

bool FunCall::hasReturn() {
	return false;
}

bool Arguments::hasReturn() {
	yyerror("fatal: não devia chegar aqui!");
	return false;
}

bool Return::hasReturn() {
	return true;
}

bool Conditional::hasReturn() {
	if(this->_else == NULL)
		return false;
	return (this->then->hasReturn() && this->_else->hasReturn());
}

bool Loop::hasReturn() {
	return this->_do;
}

bool Coertion::hasReturn() {
	yyerror("fatal: não devia chegar aqui!");
	return false;
}
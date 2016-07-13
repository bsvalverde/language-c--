#include "llvmbuilder.h"

llvm::LLVMContext &context = llvm::getGlobalContext();
llvm::IRBuilder<> Builder(context);

LlvmBuilder::LlvmBuilder() {
	this->module = new llvm::Module("Intermediary C-- code", context);

	// //testes
	// auto _main = this->buildMain();
	// this->module->dump();
	// this->run();
}

void LlvmBuilder::dump() {
	this->module->dump();
}

void LlvmBuilder::run() {
	llvm::Function* _main = this->module->getFunction("main");

	llvm::ExecutionEngine* engine;
	std::string error;

	LLVMInitializeNativeTarget();

	engine = llvm::EngineBuilder(this->module).setErrorStr(&error).create();

	if (!engine) {
		fprintf(stderr, "Could not create engine: %s\n", error.c_str());
		exit(1);
    }

     void *mainptr = engine->getPointerToFunction(_main);

    int (*result)() = (int (*)())(intptr_t)mainptr;
    std::cout << "Result of main = " << result() << std::endl;
}

//teste
llvm::Function* LlvmBuilder::buildMain() {
	llvm::Function* _main = this->createFunction("main");
	llvm::BasicBlock* mainblock = this->createBasicBlock(_main, "mainBB");

	llvm::Function* func2 = this->createFunction("func2");
	llvm::BasicBlock* blockFunc2 = this->createBasicBlock(func2, "func2BB");
	Builder.SetInsertPoint(blockFunc2);
	this->createReturn(this->buildInt(4));

	this->setInsertPoint(mainblock);
	std::vector<llvm::Value*> emptyArgs;
	auto funcRet = Builder.CreateCall(func2, llvm::makeArrayRef(emptyArgs));
	this->createReturn(funcRet);



	return _main;
}

//TODO name não é o nome da variavel, usado pra acessar depois?
//TODO só int por enquanto
llvm::AllocaInst* LlvmBuilder::storeVariable(std::string name, llvm::Value* value) {
	if(value == nullptr) {
		value = this->buildInt(0);
	}

	llvm::AllocaInst* var = Builder.CreateAlloca(llvm::Type::getInt32Ty(context), NULL, name);
	this->setVariable(value, var);
	// Builder.CreateStore(value, var);

	return var;
}

llvm::Value* LlvmBuilder::loadVariable(std::string name, llvm::AllocaInst* var) {
	return Builder.CreateLoad(var, name);
}

void LlvmBuilder::setVariable(llvm::Value* value, llvm::AllocaInst* var) {
	Builder.CreateStore(value, var);
}

//TODO Só int por enquanto
llvm::Function* LlvmBuilder::createFunction(std::string name) {
	llvm::Type* typeint = llvm::Type::getInt32Ty(context);
	llvm::FunctionType* funcType = llvm::FunctionType::get(typeint, false);

	return llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name, this->module);
}

llvm::BasicBlock* LlvmBuilder::createBasicBlock(llvm::Function* function, std::string name) {
	return llvm::BasicBlock::Create(context, name, function);
}

void LlvmBuilder::createReturn(llvm::Value* value) {
	Builder.CreateRet(value);
}

void LlvmBuilder::setInsertPoint(llvm::BasicBlock* block) {
	Builder.SetInsertPoint(block);
}

llvm::BasicBlock* LlvmBuilder::getCurrentBasicBlock() {
	return Builder.GetInsertBlock();
}

llvm::BranchInst* LlvmBuilder::createCondBranch(llvm::BasicBlock* _if, llvm::Value* cond, llvm::BasicBlock* _else) {
	return Builder.CreateCondBr(cond, _if, _else);
}

llvm::CallInst* LlvmBuilder::createFunctionCall(llvm::Function* function, llvm::ArrayRef<llvm::Value*> args) {

	return Builder.CreateCall(function, args);
}

llvm::Value* LlvmBuilder::buildInt(int value) {
	return llvm::ConstantInt::get(context, llvm::APInt(32, value));	
}

llvm::Value* LlvmBuilder::buildDouble(double value) {
	return llvm::ConstantFP::get(context, llvm::APFloat(value));	
}

llvm::Value* LlvmBuilder::buildBool(bool value) {
	return llvm::ConstantInt::get(context, llvm::APInt(1, value));
}

llvm::Value* LlvmBuilder::buildSumInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateAdd(left, right, "int sum");
}

llvm::Value* LlvmBuilder::buildSumDouble(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateFAdd(left, right, "double sum");
}

llvm::Value* LlvmBuilder::buildSubInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateSub(left, right, "int sub");
}

llvm::Value* LlvmBuilder::buildSubDouble(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateFSub(left, right, "double sub");
}

llvm::Value* LlvmBuilder::buildMulInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateMul(left, right, "int mul");
}

llvm::Value* LlvmBuilder::buildMulDouble(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateFMul(left, right, "double mul");
}

llvm::Value* LlvmBuilder::buildDivInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateSDiv(left, right, "int div");
}

llvm::Value* LlvmBuilder::buildDivDouble(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateFDiv(left, right, "double div");
}

llvm::Value* LlvmBuilder::buildOr(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateOr(left, right, "or");
}

llvm::Value* LlvmBuilder::buildAnd(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateAnd(left, right, "and");
}

llvm::Value* LlvmBuilder::buildNot(llvm::Value* value) {
	return Builder.CreateNot(value, "not");
}

llvm::Value* LlvmBuilder::buildEqInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateICmpEQ(left, right, "eq int");
}

llvm::Value* LlvmBuilder::buildNeInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateICmpNE(left, right, "ne int");
}

llvm::Value* LlvmBuilder::buildGtInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateICmpSGT(left, right, "gt int");
}

llvm::Value* LlvmBuilder::buildGeInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateICmpSGE(left, right, "ge int");
}

llvm::Value* LlvmBuilder::buildLtInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateICmpSLT(left, right, "lt int");
}

llvm::Value* LlvmBuilder::buildLeInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateICmpSLE(left, right, "le int");
}

llvm::Value* LlvmBuilder::buildNeg(llvm::Value* value) {
	return Builder.CreateNeg(value, "neg");
}
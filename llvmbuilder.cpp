#include "llvmbuilder.h"

LlvmBuilder::LlvmBuilder() {
	this->module = new llvm::Module("Intermediary C-- code", context);

	auto res = this->buildDivDouble(this->buildDouble(1.0), this->buildDouble(-2.0));
	auto _main = this->buildMain(res);

	this->run();
}

void LlvmBuilder::run() {
	this->module->dump();
}

llvm::Function* LlvmBuilder::buildMain(llvm::Value* _return) {
	llvm::Type* typeint = llvm::Type::getInt32Ty(context);
	llvm::FunctionType* typemain = llvm::FunctionType::get(typeint, false);

	llvm::Function* _main = llvm::Function::Create(typemain, llvm::Function::ExternalLinkage, "main", this->module);
	llvm::BasicBlock* mainblock = llvm::BasicBlock::Create(context, "entry", _main);

	Builder.SetInsertPoint(mainblock);

	Builder.CreateRet(_return);

	// llvm::verifyFunction(*_main);

	return _main;
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
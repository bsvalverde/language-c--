#include "llvmbuilder.h"

LlvmBuilder::LlvmBuilder() {
	this->module = new llvm::Module("Intermediary C-- code", context);

	auto res = this->buildSumInt(this->buildInt(223), this->buildInt(20));
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

llvm::Value* LlvmBuilder::buildSumInt(llvm::Value* left, llvm::Value* right) {
	return Builder.CreateAdd(left, right, "sum");
}
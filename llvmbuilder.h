#pragma once

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

static llvm::LLVMContext &context = llvm::getGlobalContext();
static llvm::IRBuilder<> Builder(context);

class LlvmBuilder {
public:
	LlvmBuilder();

	llvm::Function* buildMain(llvm::Value* _return);
	llvm::Value* buildInt(int value);
	llvm::Value* buildSumInt(llvm::Value* left, llvm::Value* right);

	// tests only
	void run();
private:
	llvm::Module* module;
	// llvm::IRBuilder<> builder;
	// llvm::LLVMContext &globalContext;
};
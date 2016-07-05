// #pragma once

// #include <iostream>

// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/IRBuilder.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/Module.h"

// #include "llvm/ExecutionEngine/ExecutionEngine.h"
// #include "llvm/ExecutionEngine/JIT.h"

// #include "llvm/Analysis/Verifier.h"

// extern llvm::LLVMContext &context;
// extern llvm::IRBuilder<> Builder;

// class LlvmBuilder {
// public:
// 	LlvmBuilder();

// 	llvm::BasicBlock* createBasicBlock(llvm::Function* function=nullptr, std::string name="");
// 	llvm::BranchInst* createCondBranch(llvm::BasicBlock* _if, llvm::Value* cond, llvm::BasicBlock* _else=nullptr);
	
// 	llvm::Value* buildInt(int value);
// 	llvm::Value* buildDouble(double value);
// 	llvm::Value* buildBool(bool value);

// 	llvm::Value* buildSumInt(llvm::Value* left, llvm::Value* right);
// 	llvm::Value* buildSumDouble(llvm::Value* left, llvm::Value* right);

// 	llvm::Value* buildSubInt(llvm::Value* left, llvm::Value* right);
// 	llvm::Value* buildSubDouble(llvm::Value* left, llvm::Value* right);

// 	llvm::Value* buildMulInt(llvm::Value* left, llvm::Value* right);
// 	llvm::Value* buildMulDouble(llvm::Value* left, llvm::Value* right);

// 	llvm::Value* buildDivInt(llvm::Value* left, llvm::Value* right);
// 	llvm::Value* buildDivDouble(llvm::Value* left, llvm::Value* right);

// 	llvm::Value* buildOr(llvm::Value* left, llvm::Value* right);
// 	llvm::Value* buildAnd(llvm::Value* left, llvm::Value* right);
// 	llvm::Value* buildNot(llvm::Value* left);

// 	// testes apenas!
// 	llvm::Function* buildMain();
// 	void run(llvm::Function* _main);
// private:

// 	llvm::Module* module;
// };
#pragma once

#include <iostream>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"

//#include "llvm/Analysis/Verifier.h"

extern llvm::LLVMContext &context;
extern llvm::IRBuilder<> Builder;

class LlvmBuilder {
public:
	LlvmBuilder();

	llvm::AllocaInst* storeVariable(std::string name, llvm::Value* value=nullptr);
	llvm::Value* loadVariable(std::string name, llvm::AllocaInst* var);
	void setVariable(llvm::Value* value, llvm::AllocaInst* var);

	llvm::Function* createFunction(std::string name);
	llvm::BasicBlock* createBasicBlock(llvm::Function* function=nullptr, std::string name="");
	llvm::BranchInst* createCondBranch(llvm::BasicBlock* _if, llvm::Value* cond, llvm::BasicBlock* _else=nullptr);
	llvm::BranchInst* createBranch(llvm::BasicBlock* dest);

	void setInsertPoint(llvm::BasicBlock* block);
	llvm::BasicBlock* getCurrentBasicBlock();

	void createReturn(llvm::Value* value);

	llvm::CallInst* createFunctionCall(llvm::Function* function, llvm::ArrayRef<llvm::Value*> args);
	
	llvm::Value* buildInt(int value);
	llvm::Value* buildDouble(double value);
	llvm::Value* buildBool(bool value);

	llvm::Value* buildSumInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildSumDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildSubInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildSubDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildMulInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildMulDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildDivInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildDivDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildOr(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildAnd(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildNot(llvm::Value* value);

	llvm::Value* buildNeg(llvm::Value* value);

	llvm::Value* buildEqInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildEqDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildNeInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildNeDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildGtInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildGtDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildGeInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildGeDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildLtInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildLtDouble(llvm::Value* left, llvm::Value* right);

	llvm::Value* buildLeInt(llvm::Value* left, llvm::Value* right);
	llvm::Value* buildLeDouble(llvm::Value* left, llvm::Value* right);


	// testes apenas!
	llvm::Function* buildMain();
	void dump();
	void run();
private:

	llvm::Module* module;
};
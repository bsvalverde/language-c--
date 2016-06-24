#include "llvmbuilder.h"

LlvmBuilder::LlvmBuilder() {
	// this->context = llvm::getGlobalContext();
	// this->builder(context);
	// this->module = new llvm::Module("Intermediary C-- code", this->context);
}

// void LlvmBuilder::buildSum(ST::Symbol left, ST::Symbol right) {
// 	auto a = left.build();
// 	auto constBuilder = llvm::APInt();
// 	if(left->type == Type::_int)

// 		auto v1 = llvm::ConstantInt::get(this->context, llvm::APInt())
// 	this->builder.createSum();
// }
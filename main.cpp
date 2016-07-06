#include <iostream>
#include "ast.h"
#include "llvmbuilder.h"
#include "symtable.h"

extern AST::Block* root; //set on Bison file
extern int yyparse();
extern int yydebug;

int main(int argc, char **argv)
{
	// if(AST::llvmbuilder == 0) {
		// AST::llvmbuilder = new LlvmBuilder();
	// }

    LlvmBuilder* llvmbuilder = new LlvmBuilder();

    //yydebug = 1;
    yyparse();
    //root->analyzeTree(); 
    std::cout << std::endl;
    root->analyzeTree(llvmbuilder);
    std::cout << std::endl;

    llvmbuilder->dump();

    return 0;
}
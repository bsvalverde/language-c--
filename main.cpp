#include <iostream>
#include "ast.h"
#include "llvmbuilder.h"
#include "symtable.h"

extern AST::Block* root; //set on Bison file
extern int yyparse();
extern int yydebug;
bool generateCode = true;

int main(int argc, char **argv)
{
	LlvmBuilder* llvmbuilder = new LlvmBuilder();
    //yydebug = 1;
    yyparse();
    if(generateCode){
        std::cout << std::endl; //necessario?
        root->analyzeTree(llvmbuilder);
        std::cout << std::endl; //necessario?
        llvmbuilder->dump();
        llvmbuilder->run();
    }
    return 0;
}
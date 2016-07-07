%{
#include <string>

#include "ast.h"
// #include "symtable.h"

AST::Block* root;
ST::SymTable* symtable = new ST::SymTable(NULL);
Type inUse = Type::desconhecido;

extern int yylex();
extern void yyerror(const char* s, ...);	
%}

%define parse.trace

%union {
	const char* _int;
	const char* _double;
	const char* _bool;
	const char* id;

	Type typeEnum;
	AST::Arguments* argList;

	AST::Node* node;
	AST::Block* block;
}

//Definição dos tokens
%token <_int> T_INT
%token <_double> T_DOUBLE
%token <_bool> T_BOOL
%token <id> T_ID
%token T_PLUS T_MULT T_SUB T_DIV T_ATTR
%token T_AND T_OR T_NOT
%token T_APAR T_FPAR 
%token T_DINT T_DREAL T_DBOOL T_DVOID
%token T_NEQ T_EQ T_GTE T_GT T_LTE T_LT 
%token T_ENDL T_COMMA
%token T_RET
%token T_IF T_ELSE
%token T_WHILE T_DO
%token T_ACH T_FCH

//Definição de tipos não-terminais
%type <block> program code cmds ccmds
%type <node> global cmd decl listvar attr expr const fun params cond loop
%type <typeEnum> type
%type <argList> args
%type <node> newscope endscope

//Precedencia de operadores
%left T_OR T_AND
%left T_NOT
%left T_EQ T_NEQ
%left T_LT T_GT T_LTE T_GTE
%left T_PLUS T_SUB
%left T_MULT T_DIV
%right U_NEG
%left U_PAR
%nonassoc error

%start program

%%

program	: code {
			root = $1;
		}
		;

code	: global {
			$$ = new AST::Block(); 
			$$->nodes.push_back($1);
		}
		| code global {
			if($2 != NULL) $1->nodes.push_back($2);
		}
		;

global  : decl T_ENDL
		| fun
        | error T_ENDL {yyerrok; $$=NULL;}
		;

decl	: type listvar {
			$$ = new AST::DeclVar($2, symtable);
		}
		| type T_ID T_ATTR expr {
			ST::Symbol* s = symtable->addVariable($2, $1);
			AST::Variable* var = new AST::Variable($2, NULL, $1, symtable);
			$$ = new AST::AssignVar(var, $4, symtable);
		}
		;

fun 	: type T_ID T_APAR newscope params T_FPAR T_ACH cmds endscope T_FCH {
			AST::Parameter* par = (AST::Parameter*)$5;
			std::list<ST::Symbol*> parameters;
			while(par != NULL){
				parameters.push_front(symtable->addVariable(par->name, par->type));
				par = (AST::Parameter*)par->next;
			}
			symtable->addFunction($2, $1, parameters);
			$$ = new AST::Function($2, $5, $8, $1);
		}
		;

type 	: T_DINT {
			inUse = Type::_int;
			$$ = Type::_int;
		}
		| T_DREAL {
			inUse = Type::_double;
			$$ = Type::_double;
		}
		| T_DBOOL {
			inUse = Type::_bool;
			$$ = Type::_bool;
		}
		| T_DVOID {
			inUse = Type::_void;
			$$ = Type::_void;
		}
		;

listvar	: T_ID {
			symtable->addVariable($1, inUse);
			$$ = new AST::Variable($1, NULL, inUse, symtable);
		}
		| listvar T_COMMA T_ID {
			symtable->addVariable($3, inUse);
			$$ = new AST::Variable($3, $1, inUse, symtable);
		}
		;

expr	: const 
		| T_ID {
			ST::Symbol* s = symtable->getVariable($1);
			$$ = new AST::Variable($1, NULL, s->type, symtable);
		}
		| T_ID T_APAR args T_FPAR {
			ST::Symbol* s = symtable->getFunction($1, $3->arguments.size());
			$$ = new AST::FunCall($1, $3, s->type);
		}
		| expr T_PLUS expr {
			$$ = new AST::BinOp($1, plus, $3);
		}
		| expr T_SUB expr {
			$$ = new AST::BinOp($1, sub, $3);
		}
		| expr T_MULT expr {
			$$ = new AST::BinOp($1, mult, $3);
		}
		| expr T_DIV expr {
			$$ = new AST::BinOp($1, _div, $3);
		}
		| expr T_EQ expr {
			$$ = new AST::BinOp($1, eq, $3);
		}
		| expr T_NEQ expr {
			$$ = new AST::BinOp($1, neq, $3);
		}
		| expr T_LT expr {
			$$ = new AST::BinOp($1, lt, $3);
		}
		| expr T_GT expr {
			$$ = new AST::BinOp($1, gt, $3);
		}
		| expr T_LTE expr {
			$$ = new AST::BinOp($1, lte, $3);
		}
		| expr T_GTE expr {
			$$ = new AST::BinOp($1, gte, $3);
		}
		| expr T_AND expr {
			$$ = new AST::BinOp($1, _and, $3);
		}
		| expr T_OR expr {
			$$ = new AST::BinOp($1, _or, $3);
		}
		| T_SUB expr %prec U_NEG { 
			$$ = new AST::UnOp(neg, $2);
		}
		| T_NOT expr { 
			$$ = new AST::UnOp(_not, $2);
		}
		| T_APAR expr T_FPAR %prec U_PAR { 
			$$ = new AST::Par($2);
		}
		;

const   : T_INT { $$ = new AST::Const($1, Type::_int); }
		| T_DOUBLE { $$ = new AST::Const($1, Type::_double); }
		| T_BOOL { $$ = new AST::Const($1, Type::_bool); }
		;

params	: type T_ID {
			$$ = new AST::Parameter($2, NULL, $1);
		}
		| params T_COMMA type T_ID {
			$$ = new AST::Parameter($4, $1, $3);
		}
		| {
			$$ = NULL;
		}
		;

cmds	: cmd { 
			$$ = new AST::Block(); 
			$$->nodes.push_back($1);
		}
		| cmds cmd {
			if($2 != NULL) $1->nodes.push_back($2);
		}
		;

args 	: expr {
			$$ = new AST::Arguments();
			$$->arguments.push_back($1);
		}
		| args T_COMMA expr {
			if($3 != NULL) $$->arguments.push_back($3);
		}
		| {
			$$ = new AST::Arguments();
		}

cmd 	: decl T_ENDL
		| attr T_ENDL
		| cond
		| loop
		// | ret
		| error T_ENDL { yyerrok; $$=NULL; } //TODO ver como faz pra acusar erro dentro de funcao
		;

attr 	: T_ID T_ATTR expr {
			ST::Symbol* symbol = symtable->getVariable($1);
			$$ = new AST::AssignVar(new AST::Variable($1, NULL, symbol->type, symtable), $3, symtable);
		}
		;

cond 	: T_IF T_APAR expr T_FPAR ccmds {
			$$ = new AST::Conditional($3, $5, NULL);
		}
		| T_IF T_APAR expr T_FPAR ccmds T_ELSE ccmds {
			$$ = new AST::Conditional($3, $5, $7);
		}
		;

ccmds	: newscope cmd endscope {
			$$ = new AST::Block(); 
			$$->nodes.push_back($2);
		}
		| T_ACH newscope cmds endscope T_FCH {
			$$ = $3;
		}

loop	: T_WHILE T_APAR expr T_FPAR T_ACH newscope cmds endscope T_ACH {
			$$ = new AST::Loop($3, $7);
		}
		;

newscope: {
			symtable = new ST::SymTable(symtable);
			$$ = NULL;
		}
		;

endscope: {
			ST::SymTable oldtable = symtable;
			symtable = symtable->superScope;
			//??delete oldtable;
			$$ = NULL;
		}
		;

%%
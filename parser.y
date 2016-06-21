%{
#include <string>

AST::Block* root;
ST::SymTable* symtable = new ST::SymTable();

extern int yylex();
extern void yyerror(const char* s, ...);	
%}

%define parse.trace

%union {
	const char* inteiro;
	const char* real;
	const char* booleano;
	const char* id;

	Type typeEnum;
	AST::Arguments* argList;

	AST::Node* node;
	AST::Block* block;
}

//Definição dos tokens
%token <inteiro> T_INT
%token <real> T_REAL
%token <booleano> T_BOOL
%token <id> T_ID
%token T_PLUS T_MULT T_SUB T_DIV T_ATTR
%token T_AND T_OR T_NOT
%token T_APAR T_FPAR 
%token T_DINT T_DREAL T_DBOOL
%token T_NEQ T_EQ T_GTE T_GT T_LTE T_LT 
%token T_ENDL T_COMMA
// %token T_DEF T_DECL T_END T_FUN T_RET
// %token T_IF T_THEN T_ELSE
// %token T_WHILE T_DO
// %token T_TYPE T_DOT

//Definição de tipos não-terminais
%type <block> program code //cmds funcmds
%type <node> global /*cmd funcmd*/ decl listvar /*attr*/ expr const //fun params cond loop composite multdecl dot
%type <typeEnum> type
// %type <argList> arglist
// %type <node> newscope endscope

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
//		| fun
        | error {yyerrok; $$=NULL;}
		;

decl	: type listvar {
			AST::Variable* var = (AST::Variable*)$2;
			while(var != NULL){
				ST::Symbol* s = symtable->getSymbol(var->name);
				s->type = $1;
				var->type = $1;
				var = (AST::Variable*)var->next;
			}
			$$ = new AST::DeclVar($2);
		}
		| type T_ID T_ATTR expr {
			ST::Symbol* s = symtable->addSymbol($2, $1);
			AST::Variable* var = new AST::Variable($2);
			var->type = $1;
			$$ = new AST::AssignVar(var, $4);
		}
		;

//fun 	: type T_ID T_APAR params T_FPAR T_ACH cmds T_FCH { }
//		;

type 	: T_DINT { $$ = Type::inteiro; }
		| T_DREAL { $$ = Type::real; }
		| T_DBOOL { $$ = Type::booleano; }
//		| T_DVOID { $$ = Type::_void; }
		;

listvar	: T_ID {
			symtable->addSymbol($1);
			$$ = new AST::Variable($1, NULL);
		}
		| listvar T_COMMA T_ID {
			symtable->addSymbol($3);
			$$ = new AST::Variable($3, $1);
		}
		;

expr	: const 
		| T_ID {
			ST::Symbol* s = symtable->getSymbol($1);
			$$ = new AST::Variable($1, NULL);
			$$->type = s->type;
		}
//		| T_ID T_APAR args T_FPAR { }
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

const   : T_INT { $$ = new AST::Const($1, Type::inteiro); }
		| T_REAL { $$ = new AST::Const($1, Type::real); }
		| T_BOOL { $$ = new AST::Const($1, Type::booleano); }
		;

// params	: {}
// 		;

// cmds	: cmd { 
// 			$$ = new AST::Block(); 
// 			$$->nodes.push_back($1);
// 		}
// 		| cmds cmd {
// 			if($2 != NULL) $1->nodes.push_back($2);
// 		}
// 		;

// cmd 	: decl T_ENDL
// 		| attr T_ENDL
// 		| cond
// 		| loop
// 		| ret
// 		;

// attr 	: T_ID T_ATTR expr { }
// 		;

// cond	: { }
// 		;

// loop	: { }
// 		;

// newscope: { }
// 		;

// endscope: { }
// 		;

%%
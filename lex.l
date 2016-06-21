%{
#include <string>
#include "ast.h"
#include "parser.h"
extern int yylineno;
void yyerror(const char* s, ...);
%}

%option noyywrap nodefault yylineno

id [a-z]+

inteiro [0-9]+
double [0-9]+\.[0-9]+
bool "true"|"false"

%%

 /* Ignorar espaços */
[ \t] ;
\n ;

 /* Tipos */
{inteiro} {
	yylval.inteiro = strdup(yytext); /*Vai precisar mudar quando fizer o interpretador*/
	return T_INT;
}
{double} {
	yylval.double = strdup(yytext);
	return T_DOUBLE;
}
{bool} {
	yylval.booleano = strdup(yytext);
	return T_BOOL;
}

 /* Operadores */
"=" 		{return T_ATTR;}
"+" 		{return T_PLUS;}
"*" 		{return T_MULT;}
"-" 		{return T_SUB;}
"/" 		{return T_DIV;}
"&&"		{return T_AND;}
"||"		{return T_OR;}
"!"			{return T_NOT;}
"("			{return T_APAR;}
")"			{return T_FPAR;}

 /* Tipos */
"int" 		{return T_DINT;}
"double" 	{return T_DREAL;}
"bool" 		{return T_DBOOL;}
"void"		{return T_DVOID;}

 /* Operadores relacionais */
"!="	 	{return T_NEQ;}
"==" 		{return T_EQ;}
">=" 		{return T_GTE;}
">" 		{return T_GT;}
"<=" 		{return T_LTE;}
"<" 		{return T_LT;}

 /* Tokens de Função */
"return"	{return T_RET;}
 
 /* Tokens de Condição */
"if"		{return T_IF;}
"then"		{return T_THEN;}
"else"		{return T_ELSE;}

 /* Tokens de Laço */
"do"		{return T_DO;}
"while"		{return T_WHILE;}

 /* Misc */
{id}	{
	yylval.id = strdup(yytext);
	return T_ID;
}
";" 		{return T_ENDL;}
","			{return T_COMMA;}
"{"			{return T_ACH;}
"}"			{return T_FCH;}

 /* Complains about every unknown character */
.		{yyerror("lexico: simbolo desconhecido: %c.", *yytext);}

%%

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "[Linha %d] Erro ", yylineno);
    std::vfprintf(stderr, s, ap);
    std::fprintf(stderr, "\n");
}
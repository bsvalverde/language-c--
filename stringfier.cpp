#include "stringfier.h"

std::string Stringfier::typeStringM(Type type) {
	switch(type) {
		case desconhecido: return "desconhecido";
		case _int: return "inteiro";
		case _double: return "real";
		case _bool: return "booleano";
		case _void: return "void";
	}
}

std::string Stringfier::typeStringF(Type type) {
	switch(type) {
		case desconhecido: return "desconhecida";
		case _int: return "inteira";
		case _double: return "real";
		case _bool: return "booleana";
		case _void: return "void";
	}
}

std::string Stringfier::binOpString(BinOperation op) {
	switch(op) {
		case plus: return "soma";
		case sub: return "subtracao";
		case mult: return "multiplicacao";
		case _div: return "divisao";
		case gt: return "maior";
		case lt: return "menor";
		case gte: return "maior ou igual";
		case lte: return "menor ou igual";
		case eq: return "igual";
		case neq: return "diferente";
		case _and: return "e";
		case _or: return "ou";
	}
}

std::string Stringfier::unOpString(UnOperation op) {
	switch(op) {
		case _not: return "nao";
		case neg: return "menos unario";
	}
}
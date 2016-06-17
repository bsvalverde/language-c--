#pragma once

enum Type {
	desconhecido,
	inteiro,
	real,
	booleano,
	_void
};

enum BinOperation {
	plus,
	sub,
	mult,
	_div,
	gt,
	lt,
	gte,
	lte,
	eq,
	neq,
	_and,
	_or
};

enum UnOperation {
	_not,
	neg
};
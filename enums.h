#pragma once

enum Type {
	desconhecido,
	_int,
	_double,
	_bool,
	_void
};

enum Kind {
	undefined,
	function,
	variable
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
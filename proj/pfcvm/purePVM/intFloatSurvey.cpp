#include<stdio.h>

struct intFloat_st
{
	char type;
	union{
		int		integer;
		float	_float;
	};
};

int main()
{
	struct intFloat_st _if;

	_if._float = 3.14;
	_if.integer = _if._float;

	return 0;
}
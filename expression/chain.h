#pragma once
typedef unsigned int unt;
typedef unsigned char uch;

class chain
{
	friend class expression;

	double number;
	uch symb;

	chain *next;
	chain *previous;

	chain(const double Num, const uch Sign);
};


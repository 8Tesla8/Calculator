#pragma once
#include "chain.h"
#include <Windows.h>


typedef unsigned int unt;
typedef unsigned char uch;

class expression
{
	char *expr;					// massive exprexxion				
	unt *error_index;			// array error index
	unt size_expr;				// size exprexxion
	unt never_index;			// index which will never be //index которого никогда не будет
	double rezult;				// number rezult
	double memory;				// number remember
	const double pi;
	const double ge;
	const uch error_symbol;
	const uch empty;
								// valid symbol
	unt size_smb;				// size array valid symbol
	uch *const symbol;			// array valid symbol					
								// mathematical signs;
	unt size_sign;				// size array valid mathematical signs
	uch *const sign;			// array valid mathematical signs		
								
	bool flag_error;			// error flags 15
	bool flag_symbol;			// invalid symbol in exprexxion
	bool flag_first;			// incorrect the first symbols in exprexxion
	bool flag_last;				// incorrect the first symbols in exprexxion
	bool flag_division_zero;
	bool flag_fraction;			// incorrect entry fraction
	bool flag_sign;				// mathematical sign of excess - математический знак лишний
	bool flag_no_sign;			// no mathematical sign near the brackets
	bool flag_sp_symbol;		// incorrect writen special symbol- неправильно написан спец симол 
	bool flag_empty_brackets;	
	bool flag_bracket;
	bool flag_open;				// More open bracket	// больше открытых скобок
	bool flag_close;			// More closed brackets // больше закрытых скобок
	bool flag_close_first;		// Close bracket met before open	// закрытая скобка встретилась раньше открытой
	bool flag_root_negative;	// under the root can not be a negative number
		
	void zeroing_check();		// zeroing flags  
	bool check();				// chek expression
	void show_valid_symbol() const; 
	// chain // double list
	bool show_it;				// If true view of operations over the course of action expression	
								// if true просмoтр ход действий операцай над выражением
	unt size_chains;
	chain *head;
	chain *tail;
	chain *error_adres;
	void transformation();		// transformation string in double or sign
	void addT(const double Num, const uch Sign); // Add in the tail // добавление в хвост
	void Print() const;
	void deleteHead();
	void deleteAll();
	
	void action();
	void action_unary(chain *&current, unt choise, chain *&begin, chain *&end);
	void action_binary(chain *&current, unt choise, chain *&begin, chain *&end);

public:
	expression();
	~expression();
	void Start();
};
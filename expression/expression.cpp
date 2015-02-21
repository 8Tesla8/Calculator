#pragma once
#include "expression.h"
#include <string.h>
#include <iostream>
#include <conio.h> //getch here
using namespace std;
typedef unsigned int unt;
typedef unsigned char uch;

enum cods { ENTER = 13, EXIT = 27, SPACE = 32 }; 

expression::expression() :
pi(3.1415926535897932384626433832),
error_symbol(19U),
size_expr(150U),
empty(177U),
memory(0),
ge(9.81),
size_chains(0U),
error_index(nullptr),
error_adres(nullptr),
show_it(true),
head(nullptr),
tail(nullptr),
size_smb(22U),
size_sign(9U),
symbol(new uch[size_smb]),
sign(new uch[size_sign])
{
	never_index = size_expr + 5U;
	//array expression

	//expr = new uch[size_expr];
	expr = new char[size_expr];
	for (unt i = 0U; i < size_expr; ++i)
		expr[i] = 0U;

	//array valid symbol
	//size_smb = 21U;	
	//symbol = new uch[size_smb];

	unt count = 0U;
	symbol[count++] = 46U;		// 0 - point 
	// numbers from zero to nine - 10 numbers 
	for (unt i = 48U; i != 58; ++i)
		symbol[count++] = i;

	symbol[count++] = 113U;		// 11 - q, square root
	symbol[count++] = 109U;		// 12 - m, print remember number 
	symbol[count++] = 112U;		// 13 - p, Pi
	symbol[count++] = 103U;		// 14 - g, acceleration of gravity
	symbol[count++] = 97U;		// 15 - a, atg 
	symbol[count++] = 99U;		// 16 - c, cos
	symbol[count++] = 105U;		// 17 - i, asin
	symbol[count++] = 107U;		// 18 - k, ctg	
	symbol[count++] = 111U;		// 19 - o, acos
	symbol[count++] = 115U;		// 20 - s, sin
	symbol[count++] = 116U;		// 21 - t, tg
	
	//mathematical signs;
	//size_sign = 9U; 
	//sign = new uch[size_sign];

	count = 0U;
	sign[count++] = 40U;		// 0 - (
	sign[count++] = 41U;		// 1 - )
	sign[count++] = 251U;		// 2 - square root
	sign[count++] = 94U;		// 3 - ^ degree, stepen
	sign[count++] = 37U;		// 4 - %
	sign[count++] = 42U;		// 5 - *
	sign[count++] = 47U;		// 6 - /
	sign[count++] = 43U;		// 7 - +
	sign[count++] = 45U;		// 8 - - minus

	//error flags
	zeroing_check();
}
expression::~expression()
{
	delete[]expr;
	delete[]symbol;
	delete[]sign;
	delete[]error_index;
}

bool expression::check()
{
	// счет количества скобок
	// Count of the number of brackets
	unt open_bracket = 0U;
	unt close_bracket = 0U;

	bool fraction = false;
	// сначала необходимо заменить все символы нужными знаками и подсчет скобок 
	// first must replace all occurrences of the desired characters
	unt last_index = 0U;
	for (unt i = 0U; i < size_expr && expr[i] != '\0'; ++i)
	{
		// square root
		if (expr[i] == 'q') 
			expr[i] = 251U;
		//подсчет скобок
		//count brackets
		else if (expr[i] == '(') 
			++open_bracket;
		else if (expr[i] == ')') 
			++close_bracket;
		// флаг для запуска проверки дробного числа 
		// Flag to start the test fractional number
		else if (expr[i] == '.')		
			fraction = true;
		// нахождение последнего символа перед '\0' для создания массивов ошибок 
		// find the last symbol before the '\0' to check
		if (expr[i + 1] == '\0') 
			last_index = i;
	}

	//создание массива ошибок 
	// create an array of errors
	if (error_index != nullptr)
		delete[]error_index;

	error_index = new unt[last_index + 5];
	for (unt i = 0U; i < last_index + 5; ++i)
		error_index[i] = never_index;

	unt count = 0U; // index array error

#pragma region bracket
	// запустится только если есть скобка 
	// Will only run if there bracket
	if (open_bracket || close_bracket) 
	{
		// проверка количества скобок 
		// check number of brackets
		unt size_bracket = 5U;
		if (open_bracket != close_bracket)
		{
			// Open brackets more than closed
			// открытых скобок больше чем закрытых 
			if (open_bracket > close_bracket)
			{
				flag_error = false;
				flag_bracket = false;
				flag_open = false;
				size_bracket += open_bracket;
			}
			// закрытых скобок больше чем открытых 
			// Closed more than open brackets
			else if (open_bracket < close_bracket) 
			{
				flag_error = false;
				flag_bracket = false;
				flag_close = false;
				size_bracket += close_bracket;
			}
		}
		else
			size_bracket += open_bracket;

		// создание массивов открытых и закрытых скобок
		// Create array of open and closed brackets
		int *A_open_bracket = new int[size_bracket];
		int *A_close_bracket = new int[size_bracket];
		
		for (unt i = 0U; i < size_bracket; ++i)
		{
			A_close_bracket[i] = -1;
			A_open_bracket[i] = never_index;
		}
		unt index_open = 0U;
		unt index_close = 0U;

		// запись индексов открытых и закрытых скобок и проверка на ошибки связаных со скобками
		// Recording of open and closed parentheses and error checking connectivity with brackets
		for (unt i = 0U; i < size_expr && expr[i] != '\0'; ++i)
		{
#pragma region open_bracket
			if (expr[i] == '(')
			{
				A_open_bracket[index_open++] = i;

				if (i == last_index)
				{
					flag_error = false;
					flag_bracket = false;
					flag_last = false;
					error_index[count++] = last_index;
				}
				if (i != last_index)  
				{
					// ошибкa две скобки рядом, пустые скобки
					// Error two brackets next to empty parentheses
					if (expr[i + 1] == ')')
					{				
						flag_error = false;
						flag_empty_brackets = false;
						error_index[count++] = i;
					}
					// если после '(' написан знак операции - ошибка
					// If after '(' written sign of operation - error
					for (unt j = 2U; j < size_sign; ++j) // missing brackets, 0 and 1 index is '(' and ')'
					{
						if (expr[i + 1] == sign[j])
						{
							//exclusion
							if (sign[j] == '-')
							{
								break;
							}
							// ошибкa лишний математический знак
							// Error once mathematical sign
							else	
							{
								flag_error = false;
								flag_sign = false;
								error_index[count++] = i + 1;
								break;
							}
						}
					}
				}
				// если перед '(' написана цифра - пропустили знак операции ошибка
				// If before '(' written digit - missed the sign of operation error
				if (i != 0)	
				{
					for (unt j = 0U; j < size_smb; ++j)
					{			
						//Между числом и открывающей скобкой отсутствует знак операции.
						// Between numbers and opening bracket missing the sign of operation.
						if (expr[i - 1] == symbol[j]) 
						{
							//exclusion
							if (symbol[j] == 's' 
								|| symbol[j] == 'i' 
								|| symbol[j] == 'c' 
								|| symbol[j] == 'o' 
								|| symbol[j] == 't' 
								|| symbol[j] == 'k' 
								|| symbol[j] == 'a'
								)
							{	
								break;
							}
							else
							{
								flag_error = false;
								flag_no_sign = false;
								error_index[count++] = i - 1;
								break;
							}
						}	
					}
				}
			}
#pragma endregion
#pragma region closed_bracket
			else if (expr[i] == ')')
			{
				A_close_bracket[index_close++] = i;
				if (i == 0)
				{
					flag_error = false;
					flag_bracket = false;
					flag_first = false;
					error_index[count++] = 0;
				}	
				if (i != last_index)
				{
					// Между закрывающей скобкой и числом отсутствует знак бинарной операции.
					// Between the closing bracket and the number of missing the sign of a binary operation.
					for (unt j = 0U; j < size_smb; ++j)
					{
						// If after ')' is the number of skipped means a sign of operation - error
						// если после ')' есть число значит пропущен знак операции - ошибка
						if (expr[i + 1] == symbol[j]) 
						{
							flag_error = false;
							flag_no_sign = false;
							error_index[count++] = i;
							break;
						}
					}
				}
				if (i != 0) 
				{
					// если перед ')' поставлен знак математической операции - ошибка  
					// If before ')' put sign mathematical operation - error
					for (unt j = 2U; j < size_sign; ++j) // missing brackets, 0 and 1 index is '(' and ')'
					{
						if (expr[i - 1] == sign[j])					 
						{
							//exclusion
							if (sign[j] == '%')
							{
								break;
							}
							// Error once mathematical sign
							// ошибкa лишний математический знак
							else	
							{
								flag_error = false;
								flag_sign = false;
								error_index[count++] = i - 1;
								break;
							}
						}
					}
				}
			}	
#pragma endregion
		}
		//проверка поставлена ли закрытая скобка раньше открытой
		// check delivered an open bracket before the open
		for (unt i = 0U; A_close_bracket[i] != -1; ++i)
		{
			// ошибку закрытая скобка встретилась раньше открытой
			// Error closing bracket met before open
			if (A_open_bracket[i] > A_close_bracket[i]) 
			{
				flag_error = false;
				flag_bracket = false;
				flag_close_first = false;
				error_index[count++] = A_close_bracket[i];
			}
		}
		delete[]A_open_bracket;
		delete[]A_close_bracket;
	}
#pragma endregion
	
#pragma region fraction
	// If there is a fractional number - check that it is correct - occurs if two points in the same number
	// если есть дробное число - проверка на его правельность - встречается ли 2 точки в одном числе 
	if (fraction) 
	{
		unt next = 0U;
		bool stop = true;

		for (unt i = 0U; i < size_expr && expr[i] != '\0'; ++i)
		{
			// найти числа после точки
			// Find the number after the decimal point
			if (expr[i] == '.') 
			{
				if (i == 0)
				{
					flag_error = false;
					flag_first = false;
					flag_fraction = false;
					error_index[count++] = 0;
				}
				else if (i==last_index)
				{
					flag_error = false;
					flag_last = false;
					flag_fraction = false;
					error_index[count++] = last_index;
				}
				// check point before the previous character
				//проверка предыдущего символа перед точки
				if (i != 0)
				{	
					// если перед точкой идет знак - ошибка
					// If the point is in front of sign - an error
					for (unt k = 0U; k < size_sign; ++k) 
					{
						if (expr[i - 1] == sign[k])
						{
							flag_error = false;
							flag_fraction = false;
							error_index[count++] = i - 1;
						}
					}
				}
				// check the following characters after the point
				//проверка следующих символов после точки
				stop = true;
				for (next = i + 1; next < size_expr && expr[next] != '\0' && stop == true; ++next)
				{
					// A fractional number found several points
					// в дробном числе встречаются несколько точек
					if (expr[next] == '.') 
					{
						flag_error = false;
						flag_fraction = false;
						error_index[count++] = next;
					}
					// If the point is sign it says about the end of a fractional number
					// если после точки идет знак это говорит об окончании дробного числа 
					for (unt k = 0U; k < size_sign && stop==true ; ++k)  
					{
						if (expr[next] == sign[k])
						{
							i = next;
							stop = false;
						}
					}
				}
			}
		}
	}
#pragma endregion
	
#pragma region symbol
	// If false flag means 2 in the expression has an invalid symbol
	// если 2 флага false значит в выражении есть недопустимый символ
	bool same_symbol = false;
	bool same_sign = false;
	// проверка на все остальные ошибки - правильные символы введены в выражении 
	// Check for all other errors - correct symbols introduced in the expression
	for (unt i = 0U; i < size_expr && expr[i] != '\0'; ++i)
	{
		// zeroing flags
		same_symbol = false;
		same_sign = false;

		//проверка на совпадение допустимых символа 	
		//checking for a match valid symbols
		for (unt j = 0U; j < size_smb; ++j)
		{
			if (expr[i] == symbol[j])
			{
				same_symbol = true;
				if (expr[i] == 'm' || expr[i] == 'p' || expr[i] == 'g')
				{
					// если перед ним и после него есть цифра - ошибка 
					// If before him and after him is a digit - error
					for (unt k = 0U; k < size_smb; ++k)
					{
						//проверка предыдущего символа 
						// check the previous symbol
						if (i != 0 && expr[i - 1] == symbol[k])
						{
							flag_error = false;
							flag_sp_symbol = false;
							error_index[count++] = i - 1;
						}
						//проверка следующего символа 
						// check the next symbol
						if (i != last_index && expr[i + 1] == symbol[k]) 
						{
							flag_error = false;
							flag_sp_symbol = false;
							error_index[count++] = i;
						}
					}
				}
				if (expr[i] == 's' 
					|| expr[i] == 'i' 
					|| expr[i] == 'c' 
					|| expr[i] == 'o' 
					|| expr[i] == 't' 
					|| expr[i] == 'k' 
					|| expr[i] == 'a'
					)
				{
					if (i == last_index)
					{
						flag_error = false;
						flag_sp_symbol = false;
						flag_last = false;
						error_index[count++] = last_index;
					}
					// If there is a symbol in front of them
					// если перед ними есть символ  
					for (unt k = 0U; k < size_smb; ++k)
					{
						if (i != 0 && expr[i - 1] == symbol[k]) //проверка предедущего символа 
						{
							flag_error = false;
							flag_sp_symbol = false;	
							error_index[count++] = i - 1;
						}
					}
					// если после них есть математический знак - ошибка 
					// If after they have a mathematical symbol - error
					for (unt k = 1U; k < size_sign; ++k) // missing bracket, 0 index is '(' 
					{
						if (expr[i + 1] == sign[k])
						{
							flag_error = false;
							flag_sp_symbol = false;
							error_index[count++] = i;
						}
					}
				}
			}
		}

		if (expr[i] == '(' || expr[i] == ')')
			same_sign = true;
		//если есть совпадение с масивом допустимых символов нету смысла проверять с масивом знаков
		// if there is a match with an array of symbols allowed no sense to check with an array of signs
		if (!same_symbol && !same_sign) 
		{
			//проверка на совпадение математических знаков 	
			//match check mathematical signs
			for (unt j = 2U; j < size_sign; ++j) // missing brackets, 0 and 1 index is '(' and ')'
			{
				if (expr[i] == sign[j])
				{
					same_sign = true;
					if (i == 0)
					{
						// exclusion
						if (expr[0] == '-') { }
						// 1 символ не должен начинатся с '+' || '/' || '*' || '%' || '^' корень
						// 1 symbol should not be a '+' || '/' || '*' || '%' || '^' root
						else
						{
							flag_error = false;
							flag_first = false;
							flag_sign = false;
							error_index[count++] = 0;
						}
					}
					// последний символ не должен заканчиватся '+' || '/' || '*' || '%' корень
					// The last symbol should not be a  '+' || '/' || '*' || '%' root
					if (i == last_index) 
					{
						flag_error = false;
						flag_last = false;
						flag_sign = false;
						error_index[count++] = last_index;
					}				
					if (expr[i] == '/' && expr[i + 1] == '0') 
					{
						// exclusion
						if ( expr[i + 2] == '.') 
						{
							// если делится на дробь
							// If divided by a fraction
						} 
						else
						{
							flag_error = false;
							flag_division_zero = false;
							error_index[count++] = i + 1;
						}				
					}
					// если напечатано 2 знака действия  
					// if printed 2 characters actions
					for (unt k = 2U; k < size_sign; ++k) // missing brackets, 0 and 1 index is '(' and ')'
					{
						if (expr[i + 1] == sign[k])
						{
							flag_error = false;
							flag_sign = false;
							error_index[count++] = i + 1;
						}
					}
				}
			}
		}
		// если нету совпадение со списком допустимых символов и знаков - символ не правельный 
		// if there is no match to the list of valid characters and symbols - symbol is not true test
		if (!same_sign && !same_symbol)
		{
			flag_error = false;
			flag_symbol = false;
			error_index[count++] = i;
		}
	}
#pragma endregion
	
	return flag_error;
}
void expression::Start()
{
	SetConsoleTitleA("Calculation v1.4");
	system("mode con cols=56 lines=37");

	int code = 0;

	cout << "\nYou want to view the action on the expression:\npress 1 - yes or 0 - no ";
	code = _getch();
	if (code == 48) // code 1
		show_it = false;
	system("cls");
	// проверка на синтаксиз 
	// checking
	do{	
		show_valid_symbol();
		cout << "Example of the correct expression:\n";
		cout << "2q(3*m+5q((-3)^6)*(45%))-t(s(45+c(24/2)/p))\n\n";
		cout << "Enter expression without SPACE:\n\n";
		//cin >> expr;
		cin.getline(expr, size_expr);

		zeroing_check();
		// если есть ошибка выводится выражение с ошибками 
		// if there is an error displayed expression with errors
		if (!check()) 
		{
			cout << endl;
			// в этом цикле выводится выражение 
			// In this cycle is derived expression
			for (unt i = 0U; expr[i] != '\0'; ++i)
			{
				cout << expr[i];
				// в этом цикле выводится символ ошибки если индекс выражения совпадает с значением в массиве индексов ошибок  
				// This loop output error symbol if the index expression coincides with the value in the array index error
				for (unt j = 0U; error_index[j] != never_index; ++j)
				{
					if (i == error_index[j])
						cout << error_symbol;
				}
			}
			// список ошибок 
			// List of errors
			cout << "\n\nError:\n";
			if (!flag_symbol)
				cout << "Invalid symbol in expression\n";
			if (!flag_sp_symbol)
				cout << "Special symbol is written incorrectly\n";
			if (!flag_division_zero)
				cout << "Division by zero\n";
			if (!flag_first)
				cout << "First symbol is incorrect\n";
			if (!flag_last)
				cout << "Last symbol is incorrect\n";
			if (!flag_sign)
				cout << "Mathematical sign of excess\n"; // математический знак лишний
			if (!flag_no_sign)
				cout << "Near the brackets no mathematical sign\n";
			if (!flag_fraction)
				cout << "Fractional number is incorrect\n";
			if (!flag_empty_brackets)
				cout << "Brackets are empty\n";
			if (!flag_bracket)
			{
				cout << "Brackets placed incorrectly\n";
				if (!flag_close_first)
					cout << "Closed bracket set before the open bracket\n";
				if (!flag_open)
					cout << "Open brackets more than closed brackets\n";
				if (!flag_close)
					cout << "Closed brackets more than open brackets\n";
			}	
			
			cout << "\nPress Space to continue or ESC to exit";
			code = _getch();

			if (code == SPACE)
			{
				system("cls");
				continue;			
			}			
			else if (code == EXIT)
			{
				system("cls");
				break;
			}				
			else
			{
				cout << "Pressed the wrong button";
				Sleep(1000);
				continue;
			}				
		}
		if (flag_error)
		{
			transformation();
			//Print();
			action();
			deleteAll();
					
			if (flag_error)
				cout << "If you wanna save rezult in memory:\npress r, Space to continue or ESC to exit";
			else if (!flag_error)
				cout << "\nPress Space to continue or ESC to exit";
			code = _getch();
			if (code == 114)// code symbol 'r'
			{
				memory=rezult;
				system("cls");
				continue;
			}
			else if (code == SPACE)
			{
				system("cls");
				continue;
			}
			else if (code == EXIT)
			{
				system("cls");
				break;
			}
			else
			{			
				cout << "Pressed the wrong button";
				Sleep(1000);
				continue;
			}		
		}			
	} while (true);
}
void expression::zeroing_check()
{
	flag_error = true;
	flag_symbol = true;
	flag_first = true;
	flag_last = true;
	flag_no_sign = true;
	flag_fraction = true;
	flag_sign = true;
	flag_division_zero = true;
	flag_sp_symbol = true;
	flag_empty_brackets = true;

	flag_bracket = true;
	flag_open = true;
	flag_close = true;
	flag_close_first = true;
	flag_root_negative = true;
}
void expression::show_valid_symbol() const
{
	cout << "Valid symbol\n";
	for (unt i = 0U; i < 10U; ++i)
		cout << symbol[i] << ' ';
	cout << "a " << "c " << "g " << "i " << "k " << "m " << "o " << "p " << "q " << "s " << "t "
		<< "\ng - acceleration of gravity"
	 << "\nm - number in memory, m = "<< memory << "\np - number pi\nq - root\n"
	 << "Unit of measurement radian"
	 << "\nc - cos  o - arccos \ns - sin  i - arcsin \nt - tg   a - arctg   \nk - ctg \n"
	 << "\nValid mathematical signs\n";
	for (unt i = 0U; i < size_sign; ++i)
		cout << sign[i] << ' ';
	cout << endl << endl;
}
void expression::transformation()
{
	double numb = 0;
	bool same = false;

	const unt Size = 20U;
	char *temp = new char[Size];
	for (unt c = 0U; c < Size; ++c)
		temp[c] = 0U;
	unt count = 0U;

	for (unt i = 0U; i < size_expr && expr[i] != '\0'; ++i)
	{
		same = false;

		for (unt j = 12U; j < size_smb; ++j) // begin from 12 - it is 'm'
		{
			if (expr[i] == symbol[j])
			{			
				if (expr[i] == 'm')
					addT(memory, empty);
				else if (expr[i] == 'p')
					addT(pi, empty);
				else if (expr[i] == 'g')
					addT(ge, empty);
				else 
					// значит символ это 's' || 'i' || 'c' || 'o' || 't' || 'k' || 'a' 
					// symbol is 's' || 'i' || 'c' || 'o' || 't' || 'k' || 'a'
					addT(0, expr[i]);
				same = true;
				break;
			}
		}

		// проверка на совпадение математических знаков 		
		//match check mathematical signs
		for (unt j = 0U; j < size_sign; ++j)
		{
			if (expr[i] == sign[j])
			{
				// записываем минусовое число - + // i==0
				// Write the minus number
				if (expr[i] == '-' && (!i || expr[i-1] == '(')) 
				{
					temp[count++] = expr[i]; // записываем минус // Write the minus 
					same = true;
					break;
				}
				else
				{		
					if (expr[i] != '('  
						&& expr[i - 1] != ')' 
						&& expr[i - 1] != 'm' 
						&& expr[i - 1] != 'p' 
						&& expr[i - 1] != 'g' 
						&& expr[i - 1] != '%'
						) 
					{
						temp[count] = '\0';
						numb = atof(temp);
						addT(numb, empty);

						numb = 0;
						// стираем temp
						// delete temp
						count = 0U;
						for (unt c = 0U; c < Size; ++c)
							temp[c] = 0U;
					}
					addT(0, expr[i]);
					same = true;
					break;
				}
			}
		}
		// If the same = false means this is number
		// если same=false значит число
		if (!same)  
		{
			temp[count++] = expr[i];
			// записываем последнее число
			// Write the last number
			if (expr[i + 1] == '\0')
			{
				temp[count] = '\0';
				numb = atof(temp);
				addT(numb, empty);

				numb = 0;
				break;
			}
		}
	}
	delete[]temp;
}
void expression::action()
{	
	error_adres = nullptr;
	chain *current = head, 
		*begin = head, 
		*end = tail;

	bool level4 = false;
	bool level3 = false;
	bool level2 = false;
	bool level1 = false;
	bool was = false;

	if (show_it)
		cout << "\nAction:\n";
	do{
		current = head;
		begin = head;
		end = tail;

		level4 = false; 
		level3 = false;
		level2 = false;
		level1 = false;
		// цикл на скобки - анализ
		// cycle on the brackets - Analysis
		while (current != nullptr)
		{
			if (current->symb == '(')
			{
				// если ( -3 ) - убрать лишние скобки - если между скобками одно число
				// If (-3) - remove unnecessary brackets - if one number between the brackets
				if (current->next->next->symb == ')') 
				{
					current = current->next;
					action_binary(current, 0, begin, end); // 0 - delete brackets
					if (show_it)
						Print();
				}
				else
					begin = current;
			}
			else if (current->symb == ')')
			{
				end = current;
				break;
			}
			else if (!level4 
				&&
				  (current->symb == 's' 
				|| current->symb == 'i' 
				|| current->symb == 'c' 
				|| current->symb == 'o' 
				|| current->symb == 't' 
				|| current->symb == 'k' 
				|| current->symb == 'a')
				)
				level4 = true;
			else if (!level3 
				&& (current->symb == 251U || current->symb == '^' || current->symb == '%' ))
				level3 = true;
			else if (!level2 
				&& (current->symb == '/' || current->symb == '*'))
				level2 = true;
			else if (!level1
				&& (current->symb == '+' || current->symb == '-'))
				level1 = true;

			current = current->next;
		}

		if (level4)
		{
			current = begin;
			// sin, asin, cos, acos, tg, ctg, atg 
			while (current != end && current != nullptr && flag_error) 
			{
				was = false;
				if (current->symb == 's')		// sin
				{
					action_unary(current, 2, begin, end);
					was = true;
				}				
				else if (current->symb == 'i')	//arcsin
				{
					action_unary(current, 3, begin, end);
					was = true;
				}					
				else if (current->symb == 'c')  //cos 
				{
					action_unary(current, 4, begin, end);
					was = true;
				}					
				else if (current->symb == 'o')	//arccos 
				{
					action_unary(current, 5, begin, end);
					was = true;
				}					
				else if (current->symb == 't')	//tg
				{
					action_unary(current, 6, begin, end);
					was = true;
				}				
				else if (current->symb == 'k')	//ctg
				{
					action_unary(current, 7, begin, end);
					was = true;
				}				
				else if (current->symb == 'a')	//arctg
				{
					action_unary(current, 8, begin, end);
					was = true;
				}					

				if (show_it && was)
					Print();
				current = current->next;
			}
		}

		if (level3)
		{
			current = begin;
			while (current != end && current != nullptr && flag_error)
			{
				was = false;
				if (current->symb == '^')
				{
					action_binary(current, 5, begin, end); // 5 '^'
					was = true;
				}			
				else if (current->symb == 251U)
				{
					action_binary(current, 6, begin, end); // 6 root
					was = true;
				}		
				// работает если написать (20%)
				// Works if you write (20%)
				else if (current->symb == '%') 
				{
					action_unary(current, 1, begin, end);
					was = true;
				}				

				if (show_it && was)
					Print();
				current = current->next;
			}
			if (!flag_error)
				break;
		}

		if (level2)
		{
			current = begin;
			while (current != end && current != nullptr && flag_error)
			{
				was = false;
				if (current->symb == '/')
				{
					action_binary(current, 3, begin, end); // 3 '/'
					was = true;
				}					
				else if (current->symb == '*')
				{
					action_binary(current, 4, begin, end); // 4 '*'
					was = true;
				}				

				if (show_it && was)
					Print();
				current = current->next;
			}
			if (!flag_error)
				break;
		}

		if (level1)
		{
			current = begin;
			while (current != end && current != nullptr && flag_error)
			{
				was = false;
				if (current->symb == '-')
				{
					action_binary(current, 2, begin, end); // 2 '-'
					was = true;				
				}
				else if (current->symb == '+')
				{
					action_binary(current, 1, begin, end); // 1 '+'
					was = true;
				}					

				if (show_it && was)
					Print();
				current = current->next;
			}
		}
	} while (size_chains != 1);

	if (size_chains == 1)
	{
		rezult = head->number;
		cout << "\nRezult = " << rezult << endl;
	}
	else if (!flag_error)
	{
		cout << "\n\nError:\n";
		if (!flag_division_zero)
			cout << "Division by zero\n";
		if (!flag_root_negative)
			cout << "Under the root can not be a negative number\n";
		Print();
	}
}
// действие 1+ 2- 3/ 4* 5root 6^ 
//action 1+ 2- 3/ 4* 5root 6^
void expression::action_binary(chain *&current, unt choise, chain *&begin, chain *&end) 
{
	double left = 0, right = 0;

	// дополнительные указатели
	// More pointers
	chain *del_next = current->next;
	chain *del_previous = current->previous;

	left = current->previous->number;
	right = current->next->number;
	// замена данных  
	// Replace data
	if (choise == 1)
		current->number = left + right;
	else if (choise == 2)
		current->number = left - right;
	else if (choise == 3)
	{
		if (!right) //==0
		{
			error_adres = current;
			flag_error = false;
			flag_division_zero = false;
			return;
		}
		else
		{
			current->number = left / right;
		}
	}

	else if (choise == 4)
		current->number = left * right;

	else if (choise == 5) // степень // degree
		current->number = pow(left, right);
	else if (choise == 6) // root
	{
		if (0>right) //==0
		{
			error_adres = current;
			flag_error = false;
			flag_root_negative = false;
			return;
		}
		else
		{
			current->number = pow(right, 1.0 / left);
		}
	}
	// Remove the extra brackets, if one number between the brackets 	// delete brackets 
	// удаление лишних скобок, если между скобками одно число 
	if (choise) 
		current->symb = empty;

	//перезапись адресов
	// address rewriting
	current->next = del_next->next;
	current->previous = del_previous->previous;

	//если после del_next есть еще obj 
	// if there is after del_next obj
	if (del_next->next != nullptr)
		del_next->next->previous = current;

	//если есть предедущий obj до del_previous 
	// if there is previous to obj del previous
	if (del_previous->previous != nullptr) 
		del_previous->previous->next = current;

	// если next and previous последнии элементы
	// If the next and previous last element
	if (del_previous == head)
		head = current;	
	if (del_next == tail)
		tail = current;

	if (del_next == end)
		end = current;
	if (del_previous == begin)
		begin = current;

	// удаляем лишнии
	// Removes excess
	delete del_next;
	delete del_previous;

	size_chains -= 2;
}
void expression::action_unary(chain *&current, unt choise, chain *&begin, chain *&end) 
{
	if (choise == 1) // %
	{
		chain *del_previous = current->previous;
		double left = 0;
		left = current->previous->number;

		current->number = left / 100;
		//перезапись адресa
		// address rewriting
		current->previous = del_previous->previous;
		//если есть предедущий obj до del_previous 
		// if there is previous to obj del previous
		if (del_previous->previous != nullptr) 
			del_previous->previous->next = current;
		// если previous последний элемент
		// If the previous last element
		if (del_previous == head)
			head = current;

		if (del_previous == begin)
			begin = current;
		
		// удаляем лишний
		// Removes excess
		delete del_previous;
	}
	// Convert degrees to radians
	// градусы переводит в радианы
	else if (choise > 1) // sin, asin, cos, acos, tg, ctg, atg 
	{
		chain *del_next = current->next;
		double right = 0;
		right = current->next->number;

		if (choise == 2) 
			current->number = sin((right*pi)/180); 
		else if (choise == 3) 
			current->number = asin((right*pi) / 180);
		else if (choise == 4) 
			current->number = cos((right*pi) / 180); 
		else if (choise == 5) 
			current->number = acos((right*pi) / 180);
		else if (choise == 6)
			current->number = tan((right*pi) / 180); 
		else if (choise == 7)	//ctg 
			current->number = (cos((right*pi) / 180) / sin((right*pi) / 180));  
		else if (choise == 8)
			current->number = atan((right*pi) / 180); 

		// address rewriting
		//перезапись адресa
		current->next = del_next->next;
		//если после del_next есть еще obj 
		// if there is after del_next obj
		if (del_next->next != nullptr)
			del_next->next->previous = current;

		// если next последний элемент
		// If the last element of the next
		if (del_next == tail)
			tail = current;

		if (del_next == end)
			end = current;
		// Removes excess
		// удаляем лишний
		delete del_next;
	}
	// замена данных  
	current->symb = empty;
	size_chains -= 1;
}
// Add in the tail 
// добавление в хвост
void expression::addT(const double Num, const uch Sign) 
{
	chain *temp = new chain(Num, Sign);

	if (size_chains>0U)
	{
		tail->next = temp;
		temp->previous = tail;
		tail = temp;
	}
	else
	{
		head = temp;
		tail = temp;
	}
	++size_chains;
}
// print выражение в динамической структуре данных
// Print the expression in a dynamic data structure
void expression::Print() const 
{
	chain* current = head;

	while (current != nullptr)
	{
		if (current->symb == empty)
			cout << current->number;
		else
			cout << current->symb;

		if (error_adres != nullptr && error_adres==current)
			cout << error_symbol;
		//постоянно двигается с головы до хвоста
		// constantly moving from head to tail
		current = current->next; 
	}
	cout << endl;
}
void expression::deleteHead()
{
	if (head != nullptr)
	{
		chain* temp = head;
		head = head->next;
		if (head != nullptr)
			head->previous = nullptr;
		else
			tail = nullptr;

		delete temp;
		--size_chains;
	}
}
void expression::deleteAll()
{
	while (head != nullptr)
		deleteHead();
}
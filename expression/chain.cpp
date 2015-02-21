#pragma once
#include "chain.h"

chain::chain(const double Num, const uch Sign) :
number(Num),
symb(Sign),
next(nullptr),
previous(nullptr)
{
}
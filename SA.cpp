// SA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Parser.h"
#include <vector>
#include <string>
using namespace std;
int main()
{
	Lexer l=LA();
	Parser p(l);
	p.Rat21F();
	
	return 0;
}


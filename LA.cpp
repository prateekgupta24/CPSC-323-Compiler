// LA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "lexer.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

int main()
{
	Lexer l;
	//testing is real function
	cout << l.isReal("-.123001")<<endl;
	cout << l.isReal("-1.123001") << endl;
	cout << l.isReal("1123.001") << endl;
	cout << l.isReal("1123.001") << endl;
	cout << l.isReal("1.") << endl;
	cout << l.isReal("1.1") << endl;

}

// LA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "lexer.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "file.h"
using namespace std;
/*
	To Do
	need more test commands to see if it is working properly - 3 text files with more than 10 line of test code
	error handling
	implement write file function from file.h


*/

int main()
{
	Lexer l;
	string txtFile;
	int state =0;
	bool comment = false;

	//get file name
	cout << "Enter File Name:";
	cin >> txtFile;
	cout << endl;

	//read txt file
	vector<string>lines = readFile(txtFile);

	// get line from lines and print out the tokens
	for (int i = 0; i < lines.size(); i++) {
		string line = lines[i];
		cout << "Input: " << line << endl;
		string word = "";

		//go through each character of the line
		for (int c = 0; c < line.length(); c++) {

			//get char from string and convert it to string
			string character (1, line[c]);

			if (character == " " || l.isSeparator(character)|| l.isOperator(character)|| c== line.length()-1) {

				if (c == line.length() - 1 && character != " "&& l.isSeparator(character)&& l.isOperator(character)) {
					word += character;
				}

				if ((word.length() != 0) && comment==false) {
					if (l.isKeyword(word)) {
						l.addToken("Keyword", word);
						word = "";
					}
					else if (l.isReal(word)) {
						l.addToken("Real", word);
						word = "";
					}
					else if (l.isInteger(word)) {
						l.addToken("Integer", word);
						word = "";
					}
					else if (l.isIdentifier(word))
					{
						l.addToken("Identifier", word);
						word = "";
					}
					else
					{
						l.addToken("Unknown", word);
						word = "";
					}

				}
				if (l.isSeparator(character)) {
					//converts char into string
					l.addToken("Separator", character);
				}
				else if (l.isOperator(character)) {

					string prevchar(1, line[c - 1]);
					string nextchar(1, line[c + 1]);

					//check for start of comment
					if ((character == "*")&& (prevchar=="/") || (character == "*") && (nextchar == "/") || (character == "/") && (nextchar == "*")) {
						//word += character;
						//cout << "true- "<<word<<endl;
						comment = true;
					}
					else if ((character == "/") && (prevchar == "*")) {//check of end of comment
						comment = false;
						word = "";
					}
					else {
						l.addToken("Operator", character);
						word = "";
					}
				}
			}
			else {
				word += character;
			}
			
		}
		cout << "Tokens\t\tLexeme" << endl;
		l.printTokens();
		cout << endl;
	}

	vector<vector<Token>> tokens = l.getAllTokens();
	writeFile(txtFile, tokens);


	system("pause");

}

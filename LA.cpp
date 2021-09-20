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

	//get file name
	cout << "Enter File Name:";
	cin >> txtFile;
	cout << endl;

	//read txt file
	vector<string>lines=readFile(txtFile);

	// get line from lines and print out the tokens
	for (int i = 0; i < lines.size(); i++) {
		string line = lines[i];
		cout <<"Input: "<< line << endl;
		string word = "";

		//go through each character of the line
		for (int c = 0; c < line.length(); c++) {
			 
			//get char from string 
			char character = line[c];

			if (character == ' '|| l.isSeparator(character)) {
				if (word.length() != 0) {
					if (l.isStartComment(word)) {
						//checks for the end of comment
						if (l.isEndComment(word)) {
							word="";
						}
					}
					else if (l.isKeyword(word)) {
						l.addToken("Keyword", word);
						word = "";
					}
					else if (l.isOperator(word)) {
						l.addToken("Operator", word);
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
					string str(1, character);
					l.addToken("Separator", str);
				}
			}
			else {
				word += character;
			}
		}
		cout << "Tokens\t\tLexeme"<<endl;
		l.printTokens();
		cout << endl;
	}
	system("pause");

}

// LA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "lexer.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "file.h"
using namespace std;

int main()
{
	Lexer l;
	string txtFile;
	bool comment = false;
	string prevop = "";//used to store an previous operator for ==,!=,<=,>=

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

				if (c == line.length() - 1 && character != " "&& !l.isSeparator(character)&& !l.isOperator(character)) {//get the last character
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
					l.addToken("Separator", character);
				}
				else if (l.isOperator(character)) {
					string prevchar ="";
					if (c != 0) { //check if character is not the first one in line
						prevchar+=line[c - 1];
					}
					else prevchar = "b";
					string nextchar(1, line[c + 1]);

					//check for start of comment
					if ((character == "*")&& (prevchar=="/") || (character == "*") && (nextchar == "/") || (character == "/") && (nextchar == "*")) {

						comment = true;
					}
					else if ((character == "/") && (prevchar == "*")) {//check for end of comment
						comment = false;
						word = "";
					}
					else if ((character == "=")&& (nextchar != "=")) { //check for "<=" , ">=","!=" "=="
						if ((prevchar == "<") || (prevchar == ">") || (prevchar == "!") || (prevchar == ">")) {
							l.addToken("Operator", prevop +character);
							prevop = "";
						}

					}
					else {
						if (!((character == "<") && (nextchar == "=")) && !((character == ">") && (nextchar == "="))) {
								l.addToken("Operator", character);
						}
						else
						{
							prevop = character;
						}

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

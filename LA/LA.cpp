// LA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "lexer.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "file.h"
#include "LA.h"
using namespace std;
string get_name() {
	return fileN;
}
Lexer LA()
{
	Lexer l;
	string txtFile;
	bool comment = false;
	int state = 0;
	int fstate = 0;

	//get file name
	cout << "Enter File Name:";
	cin >> txtFile;
	cout << endl;

	//read txt file
	vector<string>lines = readFile(txtFile);

	// get line from lines and print out the tokens
	for (int i = 0; i < lines.size(); i++) {
		string line = lines[i];
		//cout << "Input: " << line << endl;
		string word = "";

		//go through each character of the line
		for (int c = 0; c < line.length(); c++) {

			//get char from string and convert it to string
			string character (1, line[c]);
			
			//get the next char
			string nextchar(1, line[c + 1]);

			if (l.isSeparator(character)) {
				state = 2;
			}
			else if (l.isOperator(character)||character=="!") {
				string prevchar = "";
				if (c != 0) { //check if character is not the first one in line
					prevchar += line[c - 1];
				}
				else prevchar = "b";

				//check for start of comment
				if ((character == "*") && (prevchar == "/") || (character == "*") && (nextchar == "/") || (character == "/") && (nextchar == "*")) {

					comment = true;
				}
				else if ((character == "/") && (prevchar == "*")) {//check for end of comment
					comment = false;
					word = "";
				}
				else if (((character == "=") && ((nextchar == ">") || (nextchar == "="))) || ((character == "<") && (nextchar == "=")) || ((character == "!") && (nextchar == "="))) { // check for <=,=>,==,!=
					state = 0;
				}
				else if ((character == "!") && (nextchar != "=")) { //check to see if ! is by itself
					state = 3;
				}
				else {
					word += character;
					state = 1;
				}
			}
			else if ((character !=" ")&&(!l.isSeparator(nextchar)) && (!l.isOperator(nextchar)) && (c!= line.length()-1)){
					state = 0;
			}
			else {// check for keyword,identifer,int,real,unknown
				if (character != " ") {
					word += character;
				}
				state = 3;
			}

			if (comment == false) {
				switch (state) {
				case 0://add char to str
					word += character;
					break;

				case 1://operator
					l.addToken("Operator", word);
					word = "";
					break;

				case 2://seperator
					l.addToken("Separator", character);
					break;

				case 3://keyword,identifer,int,real,unknown
					//cout << "Word: " << word << endl;
					//cout << "Length: " << word.length() << endl;
					
					if (word.length() != 0) {
						if (l.isKeyword(word)) {
							fstate = 1;
						}
						else if (l.isReal(word)) {
							fstate = 4;
						}
						else if (l.isInteger(word)) {
							fstate = 3;
						}
						else if (l.isIdentifier(word))
						{
							fstate = 2;
						}
						else
						{
							fstate = 5;
						}

					}
					else {
						fstate = 0;
					}
					
					switch (fstate) {
					case 0://do nothing (character is space)
						break;
					case 1://keyword
						l.addToken("Keyword", word);
						break;
					case 2://identifier
						l.addToken("Identifier", word);
						break;
					case 3://int
						l.addToken("Integer", word);
						break;
					case 4://real
						l.addToken("Real", word);
						break;
					case 5://unknown
						l.addToken("Unknown", word);
						break;
					}
					word = "";
					break;
				}
				
			}
			
		}
		
		l.addTokens();
		
	}

	vector<vector<Token>> tokens = l.getAllTokens();
	writeTokensFile(txtFile, tokens);

	return l;

}

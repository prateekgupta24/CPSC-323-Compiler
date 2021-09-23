#pragma once
#ifndef __file_
#define __file_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "lexer.h"
using namespace std;

// reads file and makes a vector of lines
vector<string>  readFile(string filename) {

	vector<string> commandlines;
	string line;

	ifstream file(filename);
	if (file.is_open()) {
		while (getline(file, line)) {
			if (line.size() != 0 ) {
				commandlines.push_back(line);
			}
			
		}
		file.close();
	}
	else {
		string file;
		cout << "File does not exist (please make sure to add .txt)" << endl;
		cout << "Please try again: ";
		cin >> file;
		cout << endl;
		commandlines= readFile(file);
	}

	return commandlines;

}

//create a writer function to write our results
void writeFile(string filename, vector<vector<Token>>tokens) {
	
	ofstream file("Results-" + filename);

	if (file.is_open()) {
		file << "Token \tLexeme\n";
		for (int i = 0; i < tokens.size(); i++) {
			//get the token from a single line
			vector<Token> Linetokens = tokens[i];
				for (int j = 0; j < Linetokens.size(); j++) {
					// get tokens from the line
					Token token = Linetokens[j];
					file << token.token << "\t" << token.lexeme << "\n";
				}
		}
	}


}

#endif
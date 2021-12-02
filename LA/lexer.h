#pragma once
#ifndef __lexer_
#define __lexer_
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <ctype.h>

using namespace std;

struct Token {

	string token;
	string lexeme;

	Token() {

	}
	Token(string t, string l) {
		token = t;
		lexeme = l;
	}
	void print() {
		cout << token << "\t" << lexeme << "\n";
	}

	bool operator==(const Token &t)const {
		if (t.token == token) {
			if (t.lexeme==lexeme)
			{
				return true;

			}
			else {
				return false;
			}
		}
		return false;
	}
	bool operator!=(const Token &t)const {
		return !(*this==t);
	}
};

class Lexer {
private:
	vector<Token> tokens; // stores tokens for one line of code
	vector<vector<Token>> allTokens;// stores all tokens

	map<string, vector<string>> Tokenmap = { {"keyword", { "while" , "for", "integer", "if", "else", "endif",  "while", "return", "get", "put", "float","double","function","true","false","real","boolean" }},
									{"identifier", {}},
									{"operator", { "=" , "+" , "-" , "*" , "/" , "<=" , "=>" ,">" , "<", "!=","==" }},
									{"separator", {";", "(" , ")", ",", "#","{","}","[","]"}},
									{"integer", {}},
									{"real", {}},
									{"unknown", {}}
	};

public:

	bool isKeyword(string word) { //check to see if it is a keyword

		vector<string>keywords = Tokenmap["keyword"];
		for (int i = 0; i < keywords.size(); i++) {
			if (word == keywords[i]) {
				return true;
			}
		}
		return false;
	}
	bool isIdentifier(string word) { //checks to see if it is identifier
		if (isalpha(word[0])) {// first character is a letter
			return true;
		}
		return false;
	}
	bool isReal(string number) { //check to see if it is real
		//integer.integer

		for (int i = 0; i < number.length(); i++) {

			if (isalpha(number[i])) {// if has a letter
				return false;
			}
			else if ((i > 0) && (i < number.length() - 1)) {//if it it not the first or last char
				if (number[i] == '.') {
					char prev = number[i - 1];
					char next = number[i + 1];
					if ((isalnum(prev)) && (isalnum(next)))// check Integer . Integer
					{
						return true;
					}
				}
			}

		}
		return false;
	}
	bool isInteger(string number) {// checks to see if it is an integer
		if (isReal(number)) {
			return false;
		}
		else {
			for (int i = 0; i < number.length(); i++) {
				if (isalpha(number[i])) {
					return false;
				}
				else if (!isalnum(number[i])) {
					return false;
				}
			}
		}
		return true;
	}

	bool isOperator(string word) {// checks to see if it is an operator

		vector<string>operators = Tokenmap["operator"];
		for (int i = 0; i < operators.size(); i++) {
			if (word == operators[i]) {
				return true;
			}
		}
		return false;
	}

	bool isSeparator(string character) {// checks to see if it is a separatoor
		vector<string>separators = Tokenmap["separator"];
		for (int i = 0; i < separators.size(); i++) {
			if (character == separators[i]) {
				return true;
			}
		}
		return false;

	}
	void addToken(string tok, string lex) {
		Token t;
		t.token = tok;
		t.lexeme = lex;

		tokens.push_back(t);

	}
	void printTokens() {
		for (int i = 0; i < tokens.size(); i++) {
			tokens[i].print();
		}
		allTokens.push_back(tokens);
		tokens.clear();
	}
	void addTokens() {
		
		allTokens.push_back(tokens);
		tokens.clear();
	}
	vector<vector<Token>> getAllTokens() {
		return allTokens;
	}

};
#endif
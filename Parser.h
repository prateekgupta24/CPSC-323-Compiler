#pragma once
#ifndef __Parser_
#define __Parser_
#include "LA/lexer.h"
#include "LA/LA.h"
#include <vector>
#include <fstream>
#include <string>
using namespace std;
struct parseTree
{
	Token token;
	string prod_rules;

	parseTree() {
	}
	parseTree(Token t, string rule) {
		token = t;
		prod_rules = rule;
	}
	void print() {
		cout << "token\t lexeme \n";
		cout << token.token << "\t" << token.lexeme << "\n";
		cout << prod_rules << "\n";
	}

};
class Parser
{
private:
	Token current_token;
	Token  next;
	int line_pos = 0;
	int token_pos = 0;
	string rules = "";

	vector<vector<Token>>tokens;
	vector<parseTree>results;

	//symbols
	Token pound_token = Token("Separator", "#");
	Token if_token = Token("Keyword", "if");
	Token openP_token = Token("Separator", "(");
	Token closeP_token = Token("Separator", ")");
	Token endif_token = Token("Keyword", "endif");
	Token else_token = Token("Keyword", "else");
	Token return_token = Token("Keyword", "return");
	Token semicolon_token=Token("Separator", ";");
	Token colon_token=Token("Separator", ",");
	Token openC_token=Token("Separator", "{");
	Token closeC_token=Token("Separator", "}");
	Token put_token = Token("Keyword", "put");
	Token get_token = Token("Keyword", "get");
	Token equal_token=Token("Operator", "=");
	Token equalto_token = Token("Operator", "==");
	Token notequal_token = Token("Operator", "!=");
	Token less_token = Token("Operator", "<");
	Token greater_token = Token("Operator", ">");
	Token lessequal_token = Token("Operator", "<=");
	Token greaterequal_token = Token("Operator", "=>");
	Token plus_token = Token("Operator", "+");
	Token minus_token = Token("Operator", "-");
	Token multiply_token = Token("Operator", "*");
	Token division_token = Token("Operator", "/");
	Token while_token=Token("Keyword", "while");
	Token true_token=Token("Keyword", "true");
	Token false_token=Token("Keyword", "false");
	Token function_token=Token("Keyword", "function");
	Token integer_token=Token("Keyword", "integer");
	Token real_token=Token("Keyword", "real");
	Token boolean_token=Token("Keyword", "boolean");

public:
	Parser(Lexer l) {
		tokens = l.getAllTokens();
	}
	Token getToken() {
		Token token;
		if (!isempty()) {
			if (line_pos < tokens.size()) {
				vector<Token> line = tokens[line_pos];
				while (line.size() == 0) // to ignore the empty vertexs from the comments
				{
					line_pos++;
					line = tokens[line_pos];
				}
				token = line[token_pos]; 
				token_pos++;
				if (token_pos >= line.size()) {
					token_pos = 0;
					line_pos++;
				}
				int nextline = line_pos;
				int nextpos = token_pos;
				if (nextline < tokens.size()) {
					vector<Token> nline = tokens[nextline];
					while (nline.size() == 0) // to ignore the empty vertexs from the comments
					{
						nextline++;
						nline = tokens[nextline];
					}
					next = nline[nextpos];
				}else {
					next= Token("EndofFIle", "EndofFIle");
				}
				
				//token.print();
			}
			else {
				token.token = "EndofFIle";
				token.lexeme = "EndofFile";
			}
		}
		else {
			token.token = "emptyFile";
			token.lexeme = "emptyFile";
		}
		
		return token;
	}
	bool isempty() {
		if (tokens.size() == 1) {
			vector<Token> line = tokens[0];
			if (line.size() == 0) {
				return true;
			}
			else 
				return false;
		}
		return false;
	}
	void match(Token t) {
		//cout << "current token: "<<current_token.lexeme<<endl;
		//cout << "t: " << t.lexeme << endl;

		if (current_token == t) {
			results.push_back(parseTree(current_token, rules));
			rules = "";
			//cout << t.token << " " << t.lexeme << " is a match" << endl;
			current_token = getToken();
			//cout << endl;
		}
		else {
			syntax_error(current_token,t.lexeme);
		}
			
	}
	void syntax_error(Token t,string expected="") {
		Token error = Token("Error", "Error");
		string err = "ERROR: "+t.token + " " + t.lexeme + " is not Expected";
		if (expected != "") {
			err+= "\nExpected "+expected;
		}
		if (expected==semicolon_token.lexeme) {
			err += " in line " + to_string(line_pos-1) + "\n";
		}
		else if (token_pos == 0) {
			err += " in line " + to_string(line_pos) + "\n";
		}
		else {
			err+= " in line "+to_string(line_pos+1)+"\n";
		}
		
		parseTree error_statement = parseTree(error, err);
		results.push_back(error_statement);
		printResults();
		writeSAFile();
		exit(3);
	}
	void printResults() {
		if (results.size() != 0) {
			for (int i = 0; i < results.size();i++) {
				parseTree p = results[i];
				if (p.token.token == "Error") {
					cout << p.prod_rules;
				}
				else {
					p.print();
				}
			}
		}
		else {
			cout << "Empty File";
		}
	}
	void writeSAFile() {
		ofstream file("SA_Results-" + get_name());
		if (results.size() != 0) {
			for (int i = 0; i < results.size(); i++) {
				parseTree p = results[i];
				if (p.token.token == "Error") {
					file << p.prod_rules;
				}
				else {
					file << "token\t lexeme \n";
					file << p.token.token << "\t" << p.token.lexeme << "\n";
					file << p.prod_rules << "\n";
				}
			}
		}
		else {
			file << "Empty File";
		}

	}
	//R1. <Rat21F>  ::=   <Opt Function Definitions>   #  <Opt Declaration List>  <Statement List>  #
	void Rat21F() {
		rules += "<Rat21F> ->  <Opt Function Definitions>   #  <Opt Declaration List>  <Statement List>  #\n";
		//cout << "In Rat21F" << endl;
		current_token = getToken();
		opt_function_definitions();
		//cout << "In Rat21F" << endl;
		match(pound_token);
		//cout << "In Rat21F" << endl;
		opt_declaration_list();
		//cout << "In Rat21F" << endl;
		statement_list();
		//cout << "In Rat21F" << endl;
		match(pound_token);
		//cout << "In Rat21F" << endl;
		printResults();
		writeSAFile();

	}
	//R2. <Opt Function Definitions> ::= <Function Definitions>     |  <Empty>
	void opt_function_definitions() {
		//cout << "In Opt Function definitions" << endl;
		
		if (current_token == function_token) {
			rules += "<Opt Function Definitions> -> <Function Definitions>   \n";
			function_definitions();
			//cout << "In Opt Function definitions" << endl;
		}
		else {
			rules += "<Opt Function Definitions> -> <Empty>   \n";
			empty();
		}

	}
	//R3. <Function Definitions>  ::= <Function> | <Function> <Function Definitions> 
	void function_definitions() {
		//cout << "In Function definitions" << endl;
		
		if (current_token == function_token) {
			rules += "<Function Definitions>  -> <Function>  \n";
			function();
			//cout << "In Function definitions" << endl;
			while(current_token == function_token) {
				rules += "<Function Definitions>  -> <Function> <Function Definitions>  \n";
				function_definitions();
				//cout << "In Function definitions" << endl;
			}
			
		}
		else {
			//cout << "In Function definitions-error" << endl;
			syntax_error(current_token,"Function definition: 'function'");
		}

	}
	//R4. <Function> ::= function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>
	void function() {
		//cout << "In Function " << endl;
		if (current_token == function_token) {
			rules += "<Function> -> function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body> \n";
			match(function_token);
			//cout << "In Function " << endl;
			if (current_token.token == "Identifier") {
				match(current_token);
				//cout << "In Function " << endl;
				match(openP_token);
				//cout << "In Function " << endl;
				opt_parameter_list();
				//cout << "In Function " << endl;
				match(closeP_token);
				opt_declaration_list();
				body();
			}
			else {
				//cout << "In Function -error" << endl;
				syntax_error(current_token,"Function: identifier");
			}
		}

	}
	//R5. <Opt Parameter List> ::=  <Parameter List>    |     <Empty>
	void opt_parameter_list() {
		//cout << "In opt_parameter_list " << endl;
		if (current_token.token == "Identifier") {
			rules += "<Opt Parameter List>  ->  <Parameter List>  \n";
			parameter_List();
			//cout << "In opt_parameter_list " << endl;
		}
		else {
			//cout << "In opt_parameter_list -empty" << endl;
			rules += "<Opt Parameter List>  ->  <Empty>  \n";
			empty();
		}

	}
	//R6. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>
	void parameter_List() {
		//cout << "In parameter_List" << endl;
		if (current_token.token == "Identifier") {
			if (next == colon_token) {
				rules += "<Parameter List>  -> <Parameter> , <Parameter List>  \n";
			}
			else {
				rules += "<Parameter List>  -> <Parameter>  \n";
			}
			parameter();
			//cout << "In parameter_List " << endl;
			if (current_token == colon_token) {
				match(colon_token);
				//cout << "In parameter_List" << endl;
				parameter_List();
				//cout << "In parameter_List" << endl;
			}
		}
		else {
			//cout << "In parameter_List -error" << endl;
			syntax_error(current_token,"Parameter List: Identifier");
		}

	}
	//R7. <Parameter> ::=  <IDs >  <Qualifier>
	void parameter() {
		//cout << "In parameter" << endl;
		if (current_token.token == "Identifier") {
			rules += "<Parameter> ->  <IDs >  <Qualifier>  \n";
			ids();
			//cout << "In parameter" << endl;
			qualifier();
			//cout << "In parameter" << endl;
		}
		else {
			//cout << "In parameter -error" << endl;
			syntax_error(current_token,"Parameter: Identifier");
		}

	}
	//R8. <Qualifier> ::= integer    |    boolean    |  real
	 void qualifier() {
		 //cout << "In qualifier" << endl;
		 if (current_token == integer_token) {
			 rules += "<Body>  -> integer  \n";
			 match(integer_token);
			 //cout << "In qualifier" << endl;
		 }
		 else if (current_token == boolean_token) {
			 rules += "<Body>  -> boolean   \n";
			 match(boolean_token);
			 //cout << "In qualifier" << endl;
		 }
		 else if (current_token == real_token) {
			 rules += "<Body>  -> real   \n";
			 match(real_token);
			 //cout << "In qualifier" << endl;
		 }
		 else {
			 //cout << "In qualifier-error" << endl;
			 syntax_error(current_token,"Qualifier: 'integer' ,'boolean', or 'real' ");
		 }

	}
	//R9. <Body>  ::=  {  < Statement List>  }
	void body() {
		//cout << "In body" << endl;
		if (current_token == openC_token) {
			rules += "<Body>  -> {  < Statement List>  }  \n";
			match(openC_token);
			//cout << "In body" << endl;
			statement_list();
			//cout << "In body" << endl;
			match(closeC_token);
			//cout << "In body" << endl;
		}
		else {
			//cout << "In body-error" << endl;
			syntax_error(current_token,"Body: '{'");
		}

	}
	//R10. <Opt Declaration List> ::= <Declaration List>   |    <Empty>
	void opt_declaration_list() {
		//cout << "In opt_declaration_list" << endl;
		if ((current_token == integer_token) || (current_token == boolean_token) || (current_token == real_token)) {
			rules += "<Opt Declaration List> ->  <Declaration List>  \n";
			declaration_list();
			//cout << "In opt_declaration_list" << endl;
		}
		else
		{
			rules += "<Opt Declaration List> ->  <Empty>  \n";
			//cout << "In opt_declaration_list empty" << endl;
			empty();
		}

	}
	//R11. <Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List> 
	void declaration_list() {
		//cout << "In declaration_list" << endl;
		if ((current_token == integer_token) || (current_token == boolean_token) || (current_token == real_token)) {
			rules += "<Declaration List>  -> <Declaration> ; \n";
			declaration(); 
			//cout << "In declaration_list" << endl;
			match(semicolon_token);
			//cout << "In declaration_list" << endl;
			if ((current_token == integer_token) || (current_token == boolean_token) || (current_token == real_token)) {
				rules += "<Declaration List>  -> <Declaration> ; <Declaration List>  \n";
				declaration_list();
				//cout << "In declaration_list" << endl;
			}
		}
		else {
			//cout << "In declaration_list-error" << endl;
			syntax_error(current_token,"Declaration list-Qualifier: 'integer' ,'boolean', or 'real'");
		}

	}
	//R12. <Declaration> ::=   <Qualifier > <IDs>
	void declaration() {
		//cout << "In declaration" << endl;
		rules += "<Declaration> -> <Qualifier > <IDs>\n";
		if ((current_token == integer_token)||(current_token == boolean_token)||(current_token == real_token) ) {
			qualifier();
			//cout << "In declaration" << endl;
			ids();
			//cout << "In declaration" << endl;
		}
		else {
			//cout << "In declaration-error" << endl;
			syntax_error(current_token, "Declaration-Qualifier: 'integer' ,'boolean', or 'real'");
		}

	}
	//R13. <IDs> ::=     <Identifier>    | <Identifier>, <IDs>
	void ids() {
		//cout << "In ids" << endl;
		if (current_token.token == "Identifier") {
			if (next == colon_token) {
				rules += "<IDs>-><Identifier>, <IDs>\n";
			}
			else {
				rules += "<IDs>-><Identifier>\n";
			}
			match(current_token);
			//cout << "In ids" << endl;
			if (current_token == colon_token) {
				match(colon_token);
				//cout << "In ids" << endl;
				ids();
				//cout << "In ids" << endl;
			}
		}
		else {
			//cout << "In ids-error" << endl;
			syntax_error(current_token,"Ids: Identifier");
		}

	}

	//R14. <Statement List> ::=   <Statement>   | <Statement> <Statement List>
	void statement_list() {
		//cout << "In statement_list" << endl;
		if ((current_token == openC_token) || (current_token.token == "Identifier") || (current_token == if_token) || (current_token == return_token) || (current_token == put_token) || (current_token == get_token) || (current_token == while_token)) {
			rules += "<Statement List>->   <Statement> \n";
			statement();
			//cout << "In statement_list" << endl;
			if ((current_token == openC_token) || (current_token.token == "Identifier") || (current_token == if_token) || (current_token == return_token) || (current_token == put_token) || (current_token == get_token) || (current_token == while_token)) {
				rules += "<Statement List>->   <Statement> <Statement List>  \n";
				statement_list();
				//cout << "In statement_list" << endl;
			}
			
		}
		else {
			//cout << "In statement_list-error" << endl;
			syntax_error(current_token,"Statement List-Statement: '{','if','return','put','get','while', or Identifier");

		}

	}
	//R15. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>
	void statement() {
		//cout << "In statement" << endl;
		if (current_token == openC_token) {
			rules += "<Statement> ->   <Compound> \n";
			compound();
			//cout << "In statement" << endl;
		}
		else if (current_token.token == "Identifier") {
			rules += "<Statement> ->   <Assign> \n";
			assign();
			//cout << "In statement" << endl;
		}
		else if (current_token == if_token) {
			rules += "<Statement> ->   <If> \n";
			If();
			//cout << "In statement" << endl;
		}
		else if (current_token == return_token) {
			rules += "<Statement> ->   <Return> \n";
			Return();
			//cout << "In statement" << endl;
		}
		else if (current_token == put_token) {
			rules += "<Statement> ->   <Print> \n";
			print();
			//cout << "In statement" << endl;
		}
		else if (current_token == get_token) {
			rules += "<Statement> ->   <Scan> \n";
			scan();
			//cout << "In statement" << endl;
		}
		else if (current_token == while_token) {
			rules += "<Statement> ->   <While> \n";
			While();
			//cout << "In statement" << endl;
		}
		else {
			//cout << "In statementerror" << endl;
			syntax_error(current_token,"Statement: '{','if','return','put','get','while', or Identifier");
		}
	}
	//R16. <Compound> ::=   {  <Statement List>  }
	void compound() {
		//cout << "In compound" << endl;
		if (current_token == openC_token) {
			rules += "<Compound> -> {  <Statement List>  } \n";
			match(openC_token);
			//cout << "In compound" << endl;
			statement_list();
			//cout << "In compound" << endl;
			match(closeC_token);
			//cout << "In compound" << endl;
		}
		else {
			//cout << "In compound-error" << endl;
			syntax_error(current_token);
		}

	}
	//R17. <Assign> ::=     <Identifier> = <Expression> ;
	void assign() {
		//cout << "In assign" << endl;
		if (current_token.token == "Identifier") {
			rules += "<Assign> ->  <Identifier> = <Expression> ; \n";
			match(current_token);
			//cout << "In assign" << endl;
			match(equal_token);
			//cout << "In assign" << endl;
			expression();
			//cout << "In assign" << endl;
			match(semicolon_token);
			//cout << "In assign" << endl;
		}
		else {
			//cout << "In assign-error" << endl;
			syntax_error(current_token,"Assign: Identifier");
		}

	}
	//R18. <If> ::=     if  ( <Condition>  ) <Statement>   endif   |  if  ( <Condition>  ) <Statement>   else  <Statement>  endif 
	void If() {
		//cout << "In If" << endl;
		if (current_token == if_token) {
			match(if_token);
			//cout << "In If" << endl;
			match(openP_token);
			//cout << "In If" << endl;
			Condition();
			//cout << "In If" << endl;
			match(closeP_token);
			//cout << "In If" << endl;
			statement();
			//cout << "In If" << endl;
			if (current_token == else_token) {
				rules += "<If> ->  if  ( <Condition>  ) <Statement>   else  <Statement>  endif \n";
				match(else_token);
				//cout << "In If" << endl;
				statement();
				//cout << "In If" << endl;
			}
			rules += "<If> ->  if  ( <Condition>  ) <Statement>   endif \n";
			match(endif_token);
			//cout << "In If" << endl;
		}
		else{
			//cout << "In If-error" << endl;
			syntax_error(current_token,"If: 'if'");
		}


	}
	//R19. <Return> ::=  return ; |  return <Expression> ; 
	void Return() {
		//cout << "In Return" << endl;
		if (current_token == return_token) {
			if (next == semicolon_token) {
				rules += "<Return> ->  return ;\n";
			}
			else {
				rules += "<Return> ->   return <Expression> ;\n";
			}
			match(return_token);
			//cout << "In Return" << endl;
			if (current_token != semicolon_token) {
				expression();
				//cout << "In Return" << endl;
			}
			match(semicolon_token);
			//cout << "In Return" << endl;
		}
		else {
			//cout << "In Return-error" << endl;
			syntax_error(current_token,"Return: 'return'");
		}

	}
	//R20. <Print> ::=    put ( <Expression>);
	void print() {
		//cout << "In print" << endl;
		if (current_token == put_token) {
			rules += "<Print> ->  put ( <Expression>);\n";
			match(put_token);
			//cout << "In print" << endl;
			match(openP_token);
			//cout << "In print" << endl;
			expression();
			//cout << "In print" << endl;
			match(closeP_token);
			//cout << "In print" << endl;
			match(semicolon_token);
			//cout << "In print" << endl;
		}
		else {
			//cout << "In print-error" << endl;
			syntax_error(current_token,"Put: 'put'");
		}
	}
	//R21. <Scan> ::=    get ( <IDs> );
	void scan() {
		//cout << "In scan" << endl;
		if (current_token == get_token) {
			rules += "<Scan> ->    get ( <IDs> );\n";
			match(get_token);
			//cout << "In scan" << endl;
			match(openP_token);
			//cout << "In scan" << endl;
			ids();
			//cout << "In scan" << endl;
			match(closeP_token);
			//cout << "In scan" << endl;
			match(semicolon_token);
			//cout << "In scan" << endl;

		}
		else{
			//cout << "In scan-error" << endl;
			syntax_error(current_token,"Get: 'get'");
		}
	}
	//R22. <While> ::=  while ( <Condition>  )  <Statement>
	void While() {
		//cout << "In While" << endl;
		if (current_token == while_token) {
			rules += "<While> -> while ( <Condition>  )  <Statement>\n";
			//cout << "In While" << endl;
			match(while_token);
			//cout << "In While" << endl;
			match(openP_token);
			//cout << "In While" << endl;
			Condition();
			//cout << "In While" << endl;
			match(closeP_token);
			//cout << "In While" << endl;
			statement();
			//cout << "In While" << endl;
		}
		else {
			//cout << "In While-error" << endl;
			syntax_error(current_token,"While: 'while'");
		}

	}
	//R23. <Condition> ::=     <Expression>  <Relop>   <Expression>
	void Condition() {
		rules += "<Condition> -> <Expression>  <Relop>   <Expression>\n";
		//cout << "In Condition" << endl;
		expression();
		//cout << "In Condition" << endl;
		relop();
		//cout << "In Condition" << endl;
		expression();
		//cout << "In Condition" << endl;
	}
	//R24. <Relop> ::=        ==   |   !=    |   >     |   <    |  <=   |    =>
	void relop() {
		//cout << "In relop" << endl;
		if (current_token == equalto_token) {
			rules += "<Relop>-> ==\n";
			match(equalto_token);
			//cout << "In relop" << endl;
		}
		else if (current_token == notequal_token) {
			rules += "<Relop>-> !=\n";
			match(notequal_token);
			//cout << "In relop" << endl;
		}
		else if (current_token==less_token) {
			rules += "<Relop>-> <\n";
			match(less_token);
			//cout << "In relop" << endl;
		}
		else if (current_token == greater_token) {
			rules += "<Relop>-> >\n";
			match(greater_token);
			//cout << "In relop" << endl;
		}
		else if (current_token == lessequal_token) {
			rules += "<Relop>-> <=\n";
			match(lessequal_token);
			//cout << "In relop" << endl;
		}
		else if (current_token == greaterequal_token) {
			rules += "<Relop>-> =>\n";
			match(greaterequal_token);
			//cout << "In relop" << endl;
		}
		else{
			//cout << "In relop-error" << endl;
			syntax_error(current_token,"Reop: '==',  '!=',  '>',   '<', '<=', or '=>'");
		}
	}
	//R25. <Expression>  ::=    <Expression> + <Term>    | <Expression>  - <Term>    |    <Term> (left recursion)
	//<Expression> ::= <Term> <ExpressionPrime>
	//<ExpressionPrime> ::= + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <empty>
	void expression() {
		//cout << "In expression" << endl;
		if ((current_token == minus_token) || (current_token.token == "Identifier") || (current_token.token == "Integer") || (current_token == openP_token) || (current_token.token == "Real") || (current_token == true_token) || (current_token == false_token)) {
			rules += "<Expression> -> <Term> <ExpressionPrime>\n";
			term();
			//cout << "In expression" << endl;
			expressionPrime();
			//cout << "In expression" << endl;
		}
		else
		{
			//cout << "In expression -error" << endl;
			syntax_error(current_token,"Expression: '-', '(', 'true', 'false', Identifier, Real, or Integer");
		}

	}
	void expressionPrime() {
		//cout << "In expressionPrime" << endl;
		if (current_token == plus_token) {
			rules += "<ExpressionPrime> -> + <Term> <ExpressionPrime>\n";
			match(plus_token);
			//cout << "In expressionPrime" << endl;
			term();
			//cout << "In expressionPrime" << endl;
			expressionPrime();
			//cout << "In expressionPrime" << endl;
		}
		else if (current_token == minus_token) {
			rules += "<ExpressionPrime> -> - <Term> <ExpressionPrime>\n";
			match(minus_token);
			//cout << "In expressionPrime" << endl;
			term();
			//cout << "In expressionPrime" << endl;
			expressionPrime();
			//cout << "In expressionPrime" << endl;
		}
		else {
			//cout << "In expressionPrime -empty" << endl;
			rules += "<ExpressionPrime> -> <empty>\n";
			empty();
		}

	}
	//R26. <Term>    ::=      <Term>  *  <Factor>     |   <Term>  /  <Factor>     |     <Factor> (left recursion)
	// <Term> ::= <Factor> <TermPrime>
	// <TermPrime> ::= * <Factor> <TermPrime> | / <Factor> <TermPrime> | <empty>
	void term() {
		//cout << "In term" << endl;
		if ((current_token == minus_token) || (current_token.token == "Identifier") || (current_token.token == "Integer") || (current_token == openP_token) || (current_token.token == "Real") || (current_token == true_token) || (current_token == false_token)) {
			rules += "<Term> -> <Factor> <TermPrime>\n";
			factor();
			//cout << "In term" << endl;
			termPrime();
			//cout << "In term" << endl;
		}
		else
		{
			//cout << "In term-error" << endl;
			syntax_error(current_token, "Term: '-', '(', 'true', 'false', Identifier, Real, or Integer");
		}

	}
	void termPrime() {
		//cout << "In termPrime" << endl;
		if (current_token == multiply_token) {
			rules += "<TermPrime> -> * <Factor> <TermPrime>\n";
			match(multiply_token);
			//cout << "In termPrime" << endl;
			factor(); 
			//cout << "In termPrime" << endl;
			termPrime();
			//cout << "In termPrime" << endl;
		}
		else if (current_token == division_token) {
			rules += "<TermPrime> -> / <Factor> <TermPrime>\n";
			match(division_token);
			//cout << "In termPrime" << endl;
			factor();
			//cout << "In termPrime" << endl;
			termPrime();
			//cout << "In termPrime" << endl;
		}
		else {
			//cout << "In termPrime - empty" << endl;
			rules += "<TermPrime> -> <empty>\n";
			empty();
		}

	}
	//R27. <Factor> ::=      -  <Primary>    |    <Primary>
	void factor() {
		//cout << "In factor" << endl;
		if (current_token == minus_token) {
			rules += "<Factor> -> -  <Primary> \n";
			match(minus_token);
			//cout << "In factor" << endl;
		}
		else {
			rules += "<Factor> ->  <Primary> \n";
		}
		if ((current_token.token == "Identifier")|| (current_token.token == "Integer")|| (current_token == openP_token)|| (current_token.token == "Real")|| (current_token == true_token)|| (current_token == false_token)) {
			primary();
			//cout << "In factor" << endl;
		}
		else {
			//cout << "In factor-error" << endl;
			syntax_error(current_token, "Term: '(', 'true', 'false', Identifier, Real, or Integer");
		}
		
	}
	//R28. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   | <Real>  |   true   |  false
	void primary() {
		//cout << "In primary" << endl;
		if (current_token.token == "Identifier") {
			if (next == openP_token) {
				rules += "<Primary> -> <Identifier>  ( <IDs> )\n";
			}
			else {
				rules += "<Primary> -> <Identifier> \n";
			}
			match(current_token);
			//cout << "In primary" << endl;
			if (current_token == openP_token) {
				match(openP_token);
				//cout << "In primary" << endl;
				ids();
				//cout << "In primary" << endl;
				match(closeP_token);
				//cout << "In primary" << endl;
			}
		}
		else if (current_token.token == "Integer") {
			rules += "<Primary> -> <Integer>\n";
			match(current_token);
			//cout << "In primary" << endl;
		}
		else if (current_token == openP_token) {
			rules += "<Primary> -> ( <Expression> )\n";
			match(openP_token);
			//cout << "In primary" << endl;
			expression();
			//cout << "In primary" << endl;
			match(closeP_token);
			//cout << "In primary" << endl;
		}
		else if (current_token.token == "Real") {
			rules += "<Primary> -> <Real>\n";
			match(current_token);
			//cout << "In primary" << endl;
		}
		else if (current_token == true_token) {
			rules += "<Primary> -> true \n";
			match(true_token);
			//cout << "In primary" << endl;
		}
		else if (current_token == false_token) {
			rules += "<Primary> -> false \n";
			match(false_token);
			//cout << "In primary" << endl;
		}
		else {
			//cout << "In primary-error" << endl;
			syntax_error(current_token, "Term: '(', 'true', 'false', Identifier, Real, or Integer");
		}
	}
	//R29. <Empty>   ::= epsilon
	void empty() {
		//cout << "In empty" << endl;
		rules += "<Empty>->epsilon\n";
		

	}
};




#endif
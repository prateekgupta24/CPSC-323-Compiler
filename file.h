#pragma once
#ifndef __file_
#define __file_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

/*

To Do:
make a function to Create and write to text file our results 
error handling

*/

// reads file and makes a vector of lines
vector<string>  readFile(string filename) {

	vector<string> commandlines;
	string line;

	ifstream file(filename);
	if (file.is_open()) {
		while (getline(file, line)) {
			commandlines.push_back(line);
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


#endif
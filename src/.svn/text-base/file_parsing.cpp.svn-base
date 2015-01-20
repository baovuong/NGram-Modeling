/*
 * file_parsing.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: bvuong
 */

#include <iostream>
#include <fstream>
#include <vector>

#include "file_parsing.h"



using namespace std;



vector<string> fetch_words(char* file_name) {
	vector<string> words;

	// open file for reading
	ifstream file(file_name);
	string word;
	if (file.is_open()) {
		while (file.good()) {
			file >> word;
			words.push_back(word);
		}
	}

	file.close();
	return words;
}

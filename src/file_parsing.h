/*
 * file_parsing.h
 *
 *  Created on: Apr 29, 2013
 *      Author: bvuong
 */

#ifndef FILE_PARSING_H_
#define FILE_PARSING_H_

#include <vector>
#include <string>

using namespace std;


/**
 * Inputs a file, fetching each word in the file, and
 * inserting them into an array/vector.
 *
 * @param	file_name name of file for input
 * @return vector of parsed words (vector<string> type)
 */
vector<string> fetch_words(char*);




vector<char> fetch_characters(const char*);


#endif /* FILE_PARSING_H_ */

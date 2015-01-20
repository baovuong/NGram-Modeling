/**
 * @file main.cpp
 *
 *  Created on: Apr 25, 2013
 *      Author: bvuong
 */



#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <set>
#include <queue>

#include "file_parsing.h"
#include "markov_models.h"
#include "n_gram_models.h"
#include "set_theory.h"
//#include "markov_models.cpp"

using namespace std;
using namespace finite_automata;
using namespace set_theory;

// function declarations

/**
 * console interface
 *
 * @param	argc
 * @param	argv
 * @return some integer
 */
int console_interface(int argc,char* argv[]);


/**
 *
 */
void TextAnalysisProcess(int argc, char* argv[]);

void Test(int argc, char* argv[]);
void Test2(int argc, char* argv[]);


/**
 *
 */
string RandomWord();


/**
 *
 */
int RandInt(int min, int max);

/**
 * creates n-gram model from vector of strings
 *
 * @param	words
 * @param	n
 * @return NGramModel value
 */
NGramModel CreateModel(vector<string> words, int n);


/**
 *
 */
vector<string> ScanThroughModel(NGramModel model, int words);


/**
 *
 */
string BackSpace(int num);

/**
 *
 */
string StackToString(stack<string> value);

/**
 *
 */
string QueueToString(queue<string> value);

/**
 * the main program
 *
 * @param	argc
 * @param	argv
 * @return some integer
 */
int main(int argc, char* argv[]) {


	double start_time = time(NULL);
	console_interface(argc,argv);



	double end_time = time(NULL);
	cout << end_time - start_time << " seconds passed" << endl;


	return 0;
}


int console_interface(int argc, char* argv[]) {
	if (argc >= 3 ) {
		//TextAnalysisProcess(argc, argv);
		TextAnalysisProcess(argc,argv);
	}
	else {
		cout << "syntax: " << argv[0] << "<n> <multiple files...>" << endl;
	}
	return 0;
}

void Test(int argc, char* argv[]) {
	cout << "program start" << endl;
	int n;
	istringstream convert(argv[1]);
	convert >> n;
	queue<string> buffer;
	for (int i=0; i<n-1; i++) {

		buffer.push(UNKNOWN_WORD);
	}

	cout << "fetching words" << endl;
	vector<string> words = fetch_words(argv[2]);
	words.erase(words.end());

	vector<NGramState*> states;
	states.resize(words.size());
	int length = 0;
	string state_words;
	for (int i=0; i<words.size(); i++) {
		cout << "processing: " << words[i];
		cout << '\r';
		states[i] = new NGramState(n);
		state_words += words[i];
		for (int j=0; j<n-1 && !buffer.empty(); j++) {
			cout << "inserting " << buffer.front() << endl;
			state_words = state_words + " " + buffer.front();
			buffer.pop();
		}
		states[i]->AssignWords(state_words);
		cout << "state words: " << state_words << endl;
		state_words = "";
		buffer.push(words[i]);
	}
	GramTable* table = new GramTable(n,words);


	for (int i=0; i<states.size(); i++) {
		table->Add(states[i]);
	}

	cout << "there have been " << table->collision_count << " collisions" << endl;

	cout << "processed " << words.size() << " words" << endl;


	vector<NGramState*> selected = table->GramStates(states[10]->previous_words());
	cout << endl << selected.size() << " states selected" << endl;

	for (int i=0; i<selected.size(); i++) {
		for (int j=0; j<selected[i]->Words().size(); j++) {
			cout << selected[i]->Words()[j] << " ";
		}
		cout << endl;
	}

}

void TextAnalysisProcess(int argc, char* argv[]) {

	int n;
	istringstream convert(argv[1]);
	convert >> n;

	vector<string> words;
	cout << "fetching words... ";
	for (int i=2; i<argc; i++) {
		vector<string> temp = fetch_words(argv[i]);
		words.reserve(words.size() + temp.size());
		words.insert(words.end(),temp.begin(),temp.end());
	}
	cout << words.size() << " words" << endl;


	NGramModel ngram = CreateModel(words,n);
	cout << "done" << endl;

	vector<string> output = ScanThroughModel(ngram,200);
	cout << "printing" << endl;
	for (int i=0; i<output.size(); i++) {
		cout << output[i] << " ";
	}
	cout << endl;
	cout << n << "-gram perplexity: " << ngram.Perplexity() << endl;
}


NGramModel CreateModel(vector<string> words, int n) {
	vector< NGramState* > states;
	NGramModel model(n,words);
	// get all states
	for (int i=0; i<n-1; i++) {
		words.insert(words.begin(),UNKNOWN_WORD);
	}
	//OutputVector(words);
	for (int i=0; i<words.size()-(n-1); i++) {
		NGramState* state = new NGramState(n);
		string words_for_state;
		for (int j=0; j<n; j++) {
			words_for_state += words[i+j] + " ";
		}
		//cout << "words for state: " << words_for_state << endl;
		state->AssignWords(words_for_state);
		states.push_back(state);
		//cout << ".";
	}

	// put states into the model

	for (int i=0; i<states.size(); i++) {
		cout << i << ": "; OutputVector(states[i]->Words());
		cout << endl;
		model.AddState(states[i]);
	}

	cout << "set up" << endl;
	model.SetUp();
	cout << "done with set up" << endl;
	return model;
}


vector<string> ScanThroughModel(NGramModel model,int words) {
	srand(time(NULL));
	vector<string> result;
	NGramState* current_state = model.initial_states()[rand() % model.initial_states().size()];
	for (int i=0; i<words; i++) {
		//cout << current_state->value() << endl;
		result.push_back(current_state->value());
		if (current_state->final_state()) {
			current_state = model.initial_states()[rand() % model.initial_states().size()];
		}
		else {
			current_state = (NGramState*)(current_state->NextState());
		}
	}
	return result;
}


string RandomWord() {
	// word length range from 1 to 15
	// characters range from 33 to 127
	string result;
	//srand(time(NULL));
	int length = RandInt(1,15);

	for (int i=0; i<length; i++) {
		result += (char)RandInt(33,127);
	}

	return result;
}


int RandInt(int min, int max) {
	return (rand() % (max-min)) + min;
}

string BackSpace(int num) {
	string result;

	for (int i=0; i<num; i++) {
		result += '\b';
	}

	return result;
}

string StackToString(stack<string> value) {
	string result = "[[^] ";

	while (!value.empty()) {
		result += value.top();
		value.pop();
		if (!value.empty())
			result += ", ";
	}

	result += " [v]]";
	return result;
}

string QueueToString(queue<string> value) {
	string result = "[>] ";
	while (!value.empty()) {
		result += value.back() + " ";
		value.pop();
	}
	result += " [>]";
	return result;
}


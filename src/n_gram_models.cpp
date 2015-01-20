/*
 * n_gram_models.cpp
 *
 *  Created on: May 1, 2013
 *      Author: bvuong
 */

#include <stdlib.h>
#include <vector>
#include <ctype.h>
#include <iostream> // debugging
#include <math.h>
#include <sstream>
#include <set>
#include <map>

#include "n_gram_models.h"
#include "set_theory.h"

using namespace set_theory;

namespace finite_automata {


// =========== NGramState member functions ======================



NGramState::NGramState(int n=1) {
	n_ = n;
	initial_state_ = false;
	final_state_ = false;
	previous_words_ = new stack<string>();
}


NGramState::~NGramState() {
	delete previous_words_;
}

void NGramState::AssignWords(string words) {
	vector<string> word_vector = StringToVector(words);

	if (word_vector.size() > n_)
		return;

	while (word_vector.size() < n_) {
		word_vector.insert(word_vector.begin(),UNKNOWN_WORD);
	}

	for (int i=0; i<n_-1; i++) {
		previous_words_->push(word_vector[i]);
	}

	set_value(word_vector[n_-1]);

}


vector<string> NGramState::Words() {
	cout << "Words method" << endl;
	vector<string> result;
	stack<string> temp;

	cout << "is previous_words empty?" << endl;
	cout << "previous_words_: " << previous_words_ << endl;
	if (previous_words_->empty())
		cout << "yes" << endl;
	else
		cout << "no" << endl;
	while (!previous_words_->empty()) {
		temp.push(previous_words_->top());
		previous_words_->pop();
		if (temp.top() != UNKNOWN_WORD)
			result.insert(result.begin(),temp.top());
	}

	while (!temp.empty()) {
		previous_words_->push(temp.top());
		temp.pop();
	}

	result.push_back(value());

	cout << "Words method done" << endl;
	return result;
}

stack<string> NGramState::previous_words() {
	return *previous_words_;
}

bool NGramState::final_state() {
	return final_state_;
}

void NGramState::set_final_state(bool value) {
	final_state_ = value;
}

bool NGramState::initial_state() {
	return initial_state_;
}

void NGramState::set_initial_state(bool value) {
	initial_state_ = value;
}


void NGramState::OutputState() {
	cout << value() << "{" << initial_state() << "," << final_state() << "}" << "|";
	for (int i=0; i<relations().size(); i++) {
		cout << relations()[i].prob << "->" << relations()[i].state->value() << ",";
	}
	cout << endl;
}

double NGramState::Probability(vector<string> content) {
	cout << "calculating probability" << endl;
	double result = 0;


	// TODO something wrong with Words() method
	cout << "getting words" << endl;
	vector<string> words = Words();
	cout << "done" << endl;

	cout << "getting prev_words" << endl;
	vector<string> prev_words = Words();
	prev_words.erase(prev_words.end());
	cout << "done" << endl;

	result = (double)Count(words,content) / (double)Count(prev_words,content);
	cout << "done" << endl;

	return result;
}



// ============= NGramModel member functions ======================

NGramModel::NGramModel(int n=1, vector<string> content=NULL) {
	n_ = n;
	content_ = content;
	//states_table_ = new GramTable(n,content);
	states_table_ = new map<stack<string>,vector<NGramState*>* >();
}

NGramModel::~NGramModel() {

}

void NGramModel::set_content(vector<string> content_args) {
	content_ = content_args;
}

vector<NGramState*> NGramModel::states() {
	return states_;
}

vector<NGramState*> NGramModel::initial_states() {
	return initial_states_;
}

vector<NGramState*> NGramModel::final_states() {
	return final_states_;
}

void NGramModel::AddState(NGramState* state) {
	if (Contains(states_,state)) {
		cout << "found duplicate" << endl;
		return;
	}
	cout << "adding state" << endl;

	cout << "in states_" << endl;
	states_.push_back(state);
	cout << "done" << endl;

	cout << "in states_table_" << endl;
	//states_table_->Add(state);

	if ((*states_table_)[state->previous_words()] == NULL)
		(*states_table_)[state->previous_words()] = new vector<NGramState*>();
	(*states_table_)[state->previous_words()]->push_back(state);
	cout << "done" << endl;

	string word = state->value();
	// if capitalized
	if (isupper(word[0])) {
		// should be an initial state
		initial_states_.push_back(state);
		state->set_initial_state(true);
	}
	// if ending in punctuation
	else if (word[word.size()-1] == '.' ||
			word[word.size()-1] == '!' ||
			word[word.size()-1] == '?') {
		final_states_.push_back(state);
		state->set_final_state(true);
	}
}

void NGramModel::SetUp() {
	cout << "set up method" << endl;
	for (int i=0; i<states_.size(); i++) {
		states_[i]->OutputState();
		// find states for chosen state to connect to

		// setup stacks for data manipulation

		cout << "assigning next" << endl;
		stack<string> next = states_[i]->previous_words();
		cout << "done" << endl;

		stack<string> temp;

		cout << "next to temp" << endl;
		while (!next.empty()) {
			temp.push(next.top());
			next.pop();
		}
		cout << "done" << endl;


		// no need for the last value
		if (!temp.empty())
			temp.pop();

		cout << "temp to next" << endl;
		while (!temp.empty()) {
			next.push(temp.top());
			temp.pop();
		}
		cout << "done" << endl;

		// add state's value
		next.push(states_[i]->value());


		cout << "getting proper states" << endl;
		//vector<NGramState*> states = states_table_->GramStates(next);
		vector<NGramState*> states = *((*states_table_)[next]);
		cout << "done" << endl;

		// connecting


		cout << "connecting" << endl;
		// TODO something wrong here
		for (int j=0; j<states.size(); j++) {
			// calculating probability
			cout << "getting prob" << endl;
			double prob = states[j]->Probability(content_);
			cout << "prob: " << prob << endl;
			// making the connection
			states_[i]->AddTransition(prob,states[j]);
		}
		cout << "done";
	}
}

double NGramModel::Perplexity() {
	//cout << "content: "; OutputVector(content_);
	double result = 1;
	for (int i=0; i<states_.size(); i++) {
		int top,bottom;
		vector<string> top_words,bottom_words;
		top_words = states_[i]->Words();
		//cout << "top words: "; OutputVector(top_words);
		bottom_words = states_[i]->Words();
		bottom_words.erase(bottom_words.end());
		top = Count(top_words,content_);
		if (bottom_words.size() == 0) {
			bottom = content_.size()-n_+1;
		}
		else {
			bottom = Count(bottom_words,content_);
		}
		//cout << "top: " << top << endl;
		//cout << "bottom: " << bottom << endl;
		if ((double)top/bottom > 0)
			result *= (double)top/bottom;
		//OutputVector(top_words); cout << "|";OutputVector(bottom_words); cout << " ";
		//cout << top << "/" << bottom << endl;
	}

	cout << result << endl;
	result = 1.0/pow(result,1.0/content_.size());

	return result;
}




// ================ GramTable ===============================

GramTable::GramTable(int n, vector<string> words) {
	n_ = n;
	// now that it's "empty", fill it with the words

	collision_count = 0;

	set<string> indexed_words;
	int c = 1;
	for (int i=0; i < words.size(); i++) {
		if (indexed_words.count(words[i]) > 0) {
			continue;
		}
		else {
			word_index_[words[i]] = c;
			indexed_words.insert(words[i]);
			c++;
		}

	}
	word_index_["<UNK>"] = c+1;



	// 3. set up hash table
	for (int i=0; i<MAX_VALUES; i++) {
		prefix_table_[i] = NULL;
	}

	// read in words, and index them

}

GramTable::~GramTable() {
	for (int i=0; i<MAX_VALUES; i++) {
		if (prefix_table_[i] != NULL) {
			if (prefix_table_[i]->prefix != NULL)
				delete prefix_table_[i]->prefix;
			if (prefix_table_[i]->states != NULL)
				delete prefix_table_[i]->states;
			free(prefix_table_[i]);
		}

	}
}


int GramTable::Hash(stack<string> prefix) {
	ostringstream sout;
	int result=0;
	int i = 0;
	int index;

	while (!prefix.empty()) {
		string word = prefix.top();
		//cout << "word: " << word << "; index: " << word_index_[word] << endl;
		prefix.pop();

		result += word_index_[word] * pow(7,i);
		i++;
	}
	return (result) % MAX_VALUES;
}

int GramTable::Hash2(stack<string> prefix) {
	return SMALLER_MAX_VALUES - (Hash(prefix) % SMALLER_MAX_VALUES);
}

void GramTable::Add(NGramState* state) {
	stack<string> prefix = state->previous_words();

	int hash = Hash(prefix);
	//cout << "hash: " << hash << endl;
	if (prefix_table_[hash] != NULL) {
		//cout << "data location not empty" << endl;
		if (!EqualPrefixes(*(prefix_table_[hash]->prefix),prefix)) {
			// collision
			//cout << "collision" << endl;
			collision_count++;
			int hash2 = Hash2(prefix);
			do {
				hash = (hash + hash2) % MAX_VALUES;

			} while (prefix_table_[hash] != NULL);
		}
		else {
			prefix_table_[hash]->states->push_back(state);
			return;
		}
	}

	prefix_table_[hash] = (PrefixRelation*)malloc(sizeof(PrefixRelation));
	prefix_table_[hash]->prefix = new stack<string>();
	prefix_table_[hash]->states = new vector<NGramState*>();

	stack<string> temp;

	while (!prefix.empty()) {
		temp.push(prefix.top());
		prefix.pop();
	}
	while (!temp.empty()) {
		prefix_table_[hash]->prefix->push(temp.top()); // this is the problem
		temp.pop();
	}

	prefix_table_[hash]->states->push_back(state);
}


vector<NGramState*> GramTable::GramStates(stack<string> prefix) {
	cout << "gramstates method" << endl;
	// get first hash
	int hash = Hash(prefix);
	cout << "hash: " << hash << endl;
	cout << prefix_table_[hash] << endl;
	if (prefix_table_[hash] != NULL) {
		if (!EqualPrefixes(prefix,*(prefix_table_[hash]->prefix))) {
			cout << "collision in GramStates method" << endl;
			// collision, must find using second hash value
			int hash2 = Hash2(prefix);
			cout << "hash2: " << hash2 << endl;
			// loop around
			for (int i=0; i<MAX_VALUES; i++) {
				hash = (hash + hash2) % MAX_VALUES;
				if (prefix_table_[hash] != NULL) {
					if (EqualPrefixes(prefix,*(prefix_table_[hash]->prefix)))
						return *(prefix_table_[hash]->states);
				}
			}
				// cannot find prefix
			vector<NGramState*> empty_states;
			return empty_states;
		}
	}
	else {
		return *(prefix_table_[hash]->states);
	}
}

// ================ functions ===============================

int Count(vector<string> words, vector<string> content) {
	if (words.size() == 0)
		return content.size();
	//cout << "starting count." << endl;
	int count = 0;
	//cout << "entering loop" << endl;
	for (int i=0; i<content.size()-words.size() + 1; i++) {
		//cout << "i: " << i << endl;
		//cout << "content[i]: " << content[i] << "; words[0]: " << words[0] << endl;
		if (content[i] == words[0]) {
			//cout << "entering inner loop" << endl;
			int inner_count = 1;
			for (int j=1; j<words.size(); j++) {
				//cout << "content[i+j]: " << content[i+j] << endl;
				//cout << "words[j]: " << words[j] << endl;
				if (content[i+j] == words[j])
					inner_count++;
			}
			if (inner_count == words.size())
				count++;
		}
	}
	//cout << "exiting loop" << endl;
	//cout << "done with count." << endl;

	return count;
}


double WordFrequency(string phrase, string content) {
	int phrase_amount = 1;
	int content_amount = 1;
	double found = 0;
	double combinations;

	// count the words

	for (int i=0; i<phrase.size(); i++) {
		if (phrase[i] == ' ') {
			phrase_amount++;
		}
	}

	for (int i=0; i<content.size(); i++) {
		if (content[i] == ' ') {
			content_amount++;
		}
	}

	// determine how many possibitilies there are

	combinations = content_amount - phrase_amount;

	// finding the phrase in content

	int looking = content.find(phrase);

	while (looking > -1) {
		cout << content.find(phrase) << endl;
		found++;
		content = content.substr(content.find(phrase)+1);
	}

	return found/combinations;
}

vector<string> StringToVector(string words) {
	vector<string> result;
	string word;
	for (int i=0; i<words.length(); i++) {
		//cout << "looking at character: '" << words[i] << "'" << endl;
		//cout << "is '" << words[i] << "' a ws? " << isspace(words[i]) << endl;
		if (isspace(words[i]) != 0 && word.length() > 0) {

			result.push_back(word);
			word = "";
		}
		else if (!isspace(words[i])){
			word += words[i];
		}
	}
	if (word.length() > 0) {
		result.push_back(word);
	}
	return result;
}

bool EqualPrefixes(stack<string> p1, stack<string> p2) {
	if (p1.size() != p2.size())
		return false;

	while (!(p1.empty() || p2.empty())) {
		if (p1.top() != p2.top())
			return false;
		p1.pop();
		p2.pop();
	}

	return true;
}

}

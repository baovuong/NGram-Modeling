/*
 * n_gram_models.h
 *
 *  Created on: Apr 29, 2013
 *      Author: bvuong
 */

#ifndef N_GRAM_MODELS_H_
#define N_GRAM_MODELS_H_

#include <string>
#include <stack>
#include <map>

#include "markov_models.h"

using namespace std;


namespace finite_automata {


#define UNKNOWN_WORD "<UNK>"
#define MAX_VALUES 1048583
#define SMALLER_MAX_VALUES 1048573

class GramTable;
class NGramState;
class NGramModel;


/**
 * NGramState class
 */
class NGramState : public MarkovState<string> {
private:
	bool initial_state_;
	bool final_state_;

	int n_;
	stack<string>* previous_words_;
	double freq;

public:


	/**
	 *
	 */
	NGramState(int n);

	/**
	 *
	 */
	virtual ~NGramState();

	/**
	 *
	 */
	bool AssignWord(int index, string word);

	/**
	 *
	 */
	void AssignWords(string words);

	/**
	 *
	 */
	bool initial_state();

	/**
	 *
	 */
	void set_initial_state(bool value);

	/**
	 *
	 */
	bool final_state();

	/**
	 *
	 */
	void set_final_state(bool value);

	/**
	 *
	 */
	vector<string> Words();

	/**
	 *
	 */
	stack<string> previous_words();

	/**
	 *
	 */
	bool contains(NGramState* state);

	/**
	 *
	 */
	void OutputState();


	/**
	 *
	 */
	double Probability(vector<string> content);
};

/**
 *
 */
class NGramModel {

private:
	vector< NGramState* > states_;
	vector< NGramState* > initial_states_;
	vector< NGramState* > final_states_;
	vector<string> content_;
	//GramTable* states_table_;
	map< stack<string>,vector<NGramState*>* >* states_table_;
	int n_;

public:

	/**
	 * constructor for NGramModel
	 *
	 * @param n
	 */
	NGramModel(int n,vector<string> content);

	/**
	 * destructor for NGramModel
	 */
	virtual ~NGramModel();


	/**
	 * calculates perplexity of the model
	 *
	 * @return perplexity value
	 */
	double Perplexity();

	/**
	 * adds state to the model
	 *
	 * @param state
	 */
	void AddState(NGramState* state);


	/**
	 *
	 */
	void SetUp();


	/**
	 *
	 */
	void set_content(vector<string> content);

	/**
	 *
	 */
	vector<NGramState*> states();

	/**
	 *
	 */
	vector<NGramState*> initial_states();

	/**
	 *
	 */
	vector<NGramState*> final_states();

};


/**
 * This struct defines the connection between prefix and words with the prefix.
 * Used for indexing for the GramTable
 */
struct PrefixRelation {
	stack<string>* prefix;
	vector<NGramState*>* states;
};

/**
 *
 */
class GramTable {
private:
	int n_;
	PrefixRelation* prefix_table_[MAX_VALUES];
	map<string,int> word_index_;
	vector<string> words;

public:


	// for debugging
	int collision_count;

	GramTable(int n, vector<string> words);
	virtual ~GramTable();

	void Add(NGramState* new_state);
	vector<NGramState*> GramStates(stack<string> prefix);

	int Hash(stack<string> prefix);
	int Hash2(stack<string> prefix);

	int WordIndex(string word);
};

/**
 * WordFrequency calculates the probability of finding a phrase in content
 * @param phrase
 * @param content
 * @param probability (<= 1)
 */
double WordFrequency(string phrase, string content);

/**
 *
 * @param words
 * @return amount of words from input
 */
int Count(vector<string> words, vector<string> content);

/**
 *
 * @param words
 * @return vector of words
 */
vector<string> StringToVector(string words);


/**
 * @param p1
 * @param p2
 * @return bool
 */
bool EqualPrefixes(stack<string> p1, stack<string> p2);
}



#endif /* N_GRAM_MODELS_H_ */

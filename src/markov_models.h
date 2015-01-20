/*
 * markov_models.h
 *
 *  Created on: Apr 29, 2013
 *      Author: bvuong
 */

#ifndef MARKOV_MODELS_H_
#define MARKOV_MODELS_H_

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;


#define MAX_POSSIBLE_STATES 1000000

namespace finite_automata {

extern bool random_set_up;

template <class T>
class MarkovState;

template <typename T>
struct MarkovRelation {
	MarkovState<T>* state;
	double prob;
};


/**
 * MarkovState is a state for a markov model
 */
template <class T>
class MarkovState {

private:
	T value_;
	vector< MarkovRelation<T> > relations_;


	/*
	 * Sorts the states in the order of probabilities
	 */
	void SortStates();

public:

	/**
	 * Constructor for MarkovState.
	 *
	 * @param value_arg
	 */
	MarkovState(T value_arg);

	/**
	 *
	 */
	MarkovState();

	/**
	 * Destructor for MarkovState.
	 */
	~MarkovState();

	/**
	 *
	 */
	bool AddTransition(double prob, MarkovState<T>* state);

	/**
	 * getter for value
	 *
	 * @return value__
	 */
	T value();

	/**
	 * setter for value_
	 *
	 * @param new_value
	 */
	void set_value(T new_value);

	/**
	 * Outputs the next state based on probability
	 *
	 * @return MarkovState pointer to the next state
	 */
	MarkovState<T>* NextState();


	/**
	 *
	 */
	void OutputProbabilities();


	/**
	 *
	 */
	vector< MarkovRelation<T> > relations();

};

/**
 * MarkovChain class
 */
template <class T>
class MarkovChain {
private:
	vector< MarkovState<T>* > states_;
	MarkovState<T>* initial_state_;
public:
	/**
	 *
	 */
	MarkovChain();

	/**
	 *
	 */
	virtual ~MarkovChain();


};

template <class T>
MarkovState<T>::MarkovState() {

}

template <class T>
MarkovState<T>::MarkovState(T value_arg) {
	value_ = value_arg;
	if (!random_set_up) {
		srand(time(NULL));
		random_set_up = true;
	}
}


template <class T>
MarkovState<T>::~MarkovState() {
}

template <class T>
bool MarkovState<T>::AddTransition(double prob, MarkovState<T>* state) {
	// check if all probability values add up to 1 (or less)
	double sum=prob;
	for (int i=0; i<relations_.size(); i++) {
		sum += relations_[i].prob;
	}

	// if the sum will go over 1, new transition insertion should fail
	if (sum > 1)
		return false;


	// if not, add connect the state
	MarkovRelation<T> relation;
	relation.state = state;
	relation.prob = prob;

	relations_.push_back(relation);
	SortStates();
	return true;
}

template <class T>
void MarkovState<T>::SortStates() {
	// do a selection sort
	for (int i=0; i<relations_.size(); i++) {
		MarkovRelation<T> value = relations_[i];
		int hole_pos = i;
		while (hole_pos >= 0 && value.prob < relations_[hole_pos-1].prob) {
			relations_[hole_pos] = relations_[hole_pos - 1];
			hole_pos = hole_pos - 1;
		}
		relations_[hole_pos] = value;
	}
}

template <class T>
T MarkovState<T>::value() {
	return value_;
}

template <class T>
void MarkovState<T>::set_value(T new_value) {
	value_ = new_value;
}


template <class T>
MarkovState<T>* MarkovState<T>::NextState() {

	MarkovState<T>* possible_states[MAX_POSSIBLE_STATES];
	int index = 0;


	// fill possible states with states
	for (int i=0; i<relations_.size(); i++) {
		int times = relations_[i].prob * MAX_POSSIBLE_STATES;
		for (int j=0; j < times; j++) {
			possible_states[index] = relations_[i].state;
			index++;
		}
	}

	// fill up empty spaces
	while (index < MAX_POSSIBLE_STATES) {
		possible_states[index] = relations_[relations_.size()-1].state;
		index++;
	}
	// return random state
	return possible_states[rand() % MAX_POSSIBLE_STATES];
}

template <class T>
void MarkovState<T>::OutputProbabilities() {

	double sum=0;
	for (int i=0; i<relations_.size(); i++) {
		sum += relations_[i].prob;
	}

	cout << "probs:[" << sum << "] ";
	for (int i=0; i<relations_.size(); i++) {
		cout << relations_[i].prob << " ";
	}
	cout << endl;
}

template <class T>
vector< MarkovRelation<T> > MarkovState<T>::relations() {
	return relations_;
}


}



#endif /* MARKOV_MODELS_H_ */

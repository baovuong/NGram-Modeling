/*
 * set_theory.h
 *
 *  Created on: May 5, 2013
 *      Author: bvuong
 */

#ifndef SET_THEORY_H_
#define SET_THEORY_H_

#include <vector>
#include <algorithm>
#include <iostream>

namespace set_theory {

using namespace std;


/**
 * calculates the intersection of two sets
 *
 * @param a
 * @param b
 * @return intersection of a and b
 */
template <typename T>
vector<T> SetIntersection(vector<T> a, vector<T> b);

/**
 * calculates the union of two sets
 *
 * @param a
 * @param b
 * @return union of a and b
 */
template <typename T>
vector<T> SetUnion(vector<T> a, vector<T> b);

/**
 * calculates the complement of two sets.
 * essentially: c = a / b
 *
 * @param a
 * @param b
 * @return complement of a and b
 */
template <typename T>
vector<T> SetComplement(vector<T> a, vector<T> b);


/**
 *
 */
template <typename T>
bool Contains(vector<T> haystack, T needle);

/**
 *
 */
template <typename T>
bool IsSubSet(vector<T> set, vector<T> sub_set);

/**
 *
 */
template <typename T>
void OutputVector(vector<T> set);


// ======================= Implementations ============================



template <typename T>
vector<T> SetIntersection(vector<T> a, vector<T> b) {
	vector<T> result;
	sort(a.begin(),a.end());
	sort(b.begin(),b.end());
	set_intersection(a.begin(),a.end(),
			b.begin(),b.end(),back_inserter(result));
	return result;

}
template <typename T>
vector<T> SetUnion(vector<T> a, vector<T> b) {
	vector<T> result;
	sort(a.begin(),a.end());
	sort(b.begin(),b.end());
	set_union(a.begin(),a.end(),
			b.begin(),b.end(),back_inserter(result));
	return result;
}
template <typename T>
bool Contains(vector<T> haystack, T needle) {
	vector<T> needle_vector;
	needle_vector.push_back(needle);

	return SetIntersection(haystack,needle_vector).size() == 1;

}
template <typename T>
bool IsSubSet(vector<T> set, vector<T> sub_set) {
	vector<T> result = SetIntersection(set,sub_set);

	if (result.size() > 0) {
		return true;
	} else {
		return false;
	}
}
template <typename T>
vector<T> SetComplement(vector<T> a, vector<T> b) {
	vector<T> result;
	sort(a.begin(),a.end());
	sort(b.begin(),b.end());
	set_difference(a.begin(),a.end(),b.begin(),b.end(),back_inserter(result));

	return result;
}
template <typename T>
void OutputVector(vector<T> set) {
	cout << "{";
	for (int i=0; i<set.size(); i++) {
		cout << set[i];
		if (i < set.size()-1)
			cout << ", ";
	}
	cout << "}";
}


}

#endif /* SET_THEORY_H_ */

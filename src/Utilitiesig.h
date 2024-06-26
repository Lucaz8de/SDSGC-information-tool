#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/* Returns a list of strings from a comma (and space)-separated input string. */
std::vector<std::string> ParseCS(const std::string& str, bool validating);

/* Reads a text file of the form n*"heading\n comma-separated data\n empty line\n"
and returns a hash map {heading => [data]}.*/
std::unordered_map<std::string, std::vector<std::string>> ReadLists(const std::string& filename, bool validating);

/* Returns the list of headings only from ReadLists' output, i.e. the hash's keys. */
std::vector<std::string> HashKeys(std::unordered_map<std::string, std::vector<std::string>> already_read_lists);

/* Overload << for vectors using << operator for the elements, 
separated by commas. Does nothing if vec is empty. */
template<typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec);

/* This function takes a vector input and uses << operator for the elements,
each on a separate line. */
template<typename T> void PrintList(const std::vector<T> &vec);

/* Just uses copy_if to return a list. That is, given a vector and a condition,
returns a vector containing the items satisfying the condition. */
template<typename T> std::vector<T> Select(const std::vector<T> &vec, const std::function<bool(const T &)> &condition);

/* Merges the two maps by modifying m1, i.e., adds the keys of m2 into m1.
 * Note if they have keys in common, m2's will overwrite m1's!
 */
template<typename T, typename S> void Merge(std::unordered_map<T, S> &m1, const std::unordered_map<T, S> &m2, bool validating);

/* Checks the size and data types of a list of data. Throws an error if they are different from what they should be.
*/
void ValidateList(const std::vector<std::string> &data, int data_size, std::vector<int> numerical_data, std::vector<int> boolean_data);
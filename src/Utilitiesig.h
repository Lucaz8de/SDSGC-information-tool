#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/* Returns a list of strings from a comma (and space)-separated input string. */
std::vector<std::string> ParseCS(const std::string& str);

/* Reads a text file of the form n*"heading\n comma-separated data\n empty line\n"
and returns a hash map {heading => [data]}.*/
std::unordered_map<std::string, std::vector<std::string>> ReadLists(const std::string& filename);

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
template<typename T> std::vector<T> select(const std::vector<T> &vec, const std::function<bool(const T &)> &condition);
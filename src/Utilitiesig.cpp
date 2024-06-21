#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Hero.h"
#include "Utilitiesig.h"

std::vector<std::string> ParseCS(const std::string& str) {
	std::vector<std::string> out{};

	std::stringstream ss{ str }; // Make a stream out of str
	std::string s{}; // Receptable for string
	std::getline(ss, s, ','); // Reads ss up to the first ',' and puts it in s
	out.push_back(s); // Add s to list

	while (ss.good()) { // Until end or fail
		ss.ignore(1); // Skip 1 character, the space
		std::getline(ss, s, ',');
		out.push_back(s);
	}
	return out;
}

std::unordered_map<std::string, std::vector<std::string>> ReadLists(const std::string& filename) {
	std::unordered_map<std::string, std::vector<std::string>> out{};

	std::ifstream file{ filename }; // Make a filestream out of filename
	std::string heading{}; // Receptacle for heading
	std::string data{}; // Receptacle for data
	while (file.good()) {
		// heading
		heading = "";
		std::getline(file, heading, '\n'); // puts the whole line in heading
		// comma-separated data
		std::string line{};
		std::getline(file, line, '\n'); // puts the whole line in line
		if (out.find(heading) != out.end()) {
			throw std::invalid_argument("The heading names in data/acquisition.txt and data/draws.txt must not contain duplicates.");
		}
		out[heading] = ParseCS(line); // adds the list to the hashmap
		// empty line
		file.ignore();
	}
	return out;
}

std::vector<std::string> HashKeys(std::unordered_map<std::string, std::vector<std::string>> already_read_lists)
{
	std::vector<std::string> out{};
	for (auto const &entry : already_read_lists) {
		std::string key = entry.first;
		out.push_back(key);
	}
	return out;
}

template<typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
	if (vec.empty()) {
		return os;
	}

	for (size_t i{ 0 }; i < vec.size() - 1; i++) {
		os << vec[i] << ", ";
	}
	os << vec[vec.size() - 1];
	return os;
}

/* Template function MUST be instantiated in the same file as it is defined.
This line of code allows this function to be used in other files now.
Ignore the warning! It is not true. This line of code works and is necessary. */
template std::ostream& operator<< <std::string>(std::ostream& os, const std::vector<std::string>& vec);

template<typename T> void PrintList(const std::vector<T>& vec) {
	for (const T& el : vec) {
		std::cout << el << std::endl;
	}
}

/* Template function MUST be instantiated in the same file as it is defined.
This line of code allows this function to be used in other files now. */
template void PrintList<Hero>(const std::vector<Hero>& vec);
template void PrintList<std::string>(const std::vector<std::string> &vec);

template<typename T> std::vector<T> select(const std::vector<T> &vec, const std::function<bool(const T &)> &condition) {
	std::vector<T> selection{ vec.size() };
	const auto it = copy_if(vec.begin(), vec.end(), selection.begin(), condition);
	selection.resize(std::distance(selection.begin(), it));
	return selection;
}
template std::vector<Hero> select(const std::vector<Hero> &vec, const std::function<bool(const Hero &)> &condition);

template<typename T, typename S> void merge(std::unordered_map<T, S> &m1, const std::unordered_map<T, S> &m2) {
	for(auto& item: m2) { // item is {key: value}
		if (m1.find(item.first) != m1.end()) {
			throw std::invalid_argument("The heading names in data/acquisition.txt and data/draws.txt must not contain duplicates.");
		}

		m1[item.first] = item.second;
	}
}
template void merge(std::unordered_map<std::string, std::vector<std::string>> &acquisition, const std::unordered_map<std::string, std::vector<std::string>> &draws);
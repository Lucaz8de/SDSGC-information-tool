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

std::vector<std::string> ParseCS(const std::string& str, bool validating) {
	std::vector<std::string> out{};

	std::stringstream ss{ str }; // Make a stream out of str

	while (ss.good()) { // Until end or fail
		std::string s{}; // Receptable for string
		std::getline(ss, s, ','); // Reads ss up to the first ',' and puts it in s
		out.push_back(s); // Add s to list
		s = ss.get(); // Reads the next character of ss and puts it in s
		if(ss.eof()) { // End of line has been reached
			break;
		}
		if(validating && s != " ") {
			std::string error_message = "Invalid format of data file on line: " + ss.str() + ". Expected lists to be separated by commas and spaces.";
			throw std::runtime_error(error_message);
		}
	}
	return out;
}

std::unordered_map<std::string, std::vector<std::string>> ReadLists(const std::string& filename, bool validating) {
	std::unordered_map<std::string, std::vector<std::string>> out{};

	std::ifstream file{ filename }; // Make a filestream out of filename
	if(!file.good()) {
		throw std::runtime_error("data files not found. Please make sure you're running the program in the install directory.");
	}
	while (file.good()) {
		// heading
		std::string heading{};
		std::getline(file, heading, '\n'); // puts the whole line in heading
		// comma-separated data
		std::string data{};
		std::getline(file, data, '\n'); // puts the whole line in line
		if (validating && out.find(heading) != out.end()) {
			throw std::runtime_error("The heading names in data/acquisition.txt and data/draws.txt must not contain duplicates.");
		}
		out[heading] = ParseCS(data, validating); // adds the list to the hashmap
		// empty line
		std::getline(file, heading, '\n'); // puts the whole line in heading, just temporarily
		if(validating && !heading.empty()) {
			throw std::runtime_error("Invalid format of data/acquisition.txt and data/draws.txt -- expected HEADING, DATA, EMPTY LINE");
		}
	}
	file.close();
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

template<typename T> std::vector<T> Select(const std::vector<T> &vec, const std::function<bool(const T &)> &condition) {
	std::vector<T> selection{ vec.size() };
	const auto it = copy_if(vec.begin(), vec.end(), selection.begin(), condition);
	selection.resize(std::distance(selection.begin(), it));
	return selection;
}
template std::vector<Hero> Select(const std::vector<Hero> &vec, const std::function<bool(const Hero &)> &condition);

template<typename T, typename S> void Merge(std::unordered_map<T, S> &m1, const std::unordered_map<T, S> &m2, bool validating) {
	for(auto& item: m2) { // item is {key: value}
		if (validating && m1.find(item.first) != m1.end()) {
			throw std::runtime_error("The heading names in data/acquisition.txt and data/draws.txt must not contain duplicates.");
		}

		m1[item.first] = item.second;
	}
}
template void Merge(std::unordered_map<std::string, std::vector<std::string>> &acquisition, const std::unordered_map<std::string, std::vector<std::string>> &draws, bool validating);

void ValidateList(const std::vector<std::string> &data, int data_size, std::vector<int> numerical_data, std::vector<int> boolean_data) {
	std::string hero = data[0];
	if (data.size() != data_size) {
		std::string error_message = "Invalid format in data file. Hero " + hero + " has " + std::to_string(data.size()) + " items instead of " + std::to_string(data_size) + ".";
		throw std::runtime_error(error_message);
	}

	for(int i = 0; i < data_size; i++) {
		// check if it's a number
		try {
			std::stoi(data[i]); // is a number
			if(std::find(numerical_data.begin(), numerical_data.end(), i) == numerical_data.end()) { // not numerical data
				std::string error_message = "Invalid data in data/owned.txt. Item \"" + data[i] + "\" of hero " + hero + " should not be a number.";
				throw std::runtime_error(error_message);
			} else {
				// fine 
			}
		} catch (std::invalid_argument) { // not a number
			if(std::find(numerical_data.begin(), numerical_data.end(), i) != numerical_data.end()) { // is numerical data
				std::string error_message = "Invalid data in data/owned.txt. Item \"" + data[i] + "\" of hero " + hero + " should be a number.";
				throw std::runtime_error(error_message);
			} else {
				// fine 
			}
		}
		// check if it's a boolean
		if(data[i] == "true" || data[i] == "false") { // is a boolean
			if(std::find(boolean_data.begin(), boolean_data.end(), i) == boolean_data.end()) { // not boolean data
				std::string error_message = "Invalid data in data/owned.txt. Item \"" + data[i] + "\" of hero " + hero + " should not be a boolean.";
				throw std::runtime_error(error_message);
			} else {
				// fine 
			}
		} else { // not a boolean
			if(std::find(boolean_data.begin(), boolean_data.end(), i) != boolean_data.end()) { // is boolean data
				std::string error_message = "Invalid data in data/owned.txt. Item \"" + data[i] + "\" of hero " + hero + " should be a boolean.";
				throw std::runtime_error(error_message);
			} else {
				// fine 
			}
		}
	}
}
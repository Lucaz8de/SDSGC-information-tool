#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Hero.h"
#include "Utilities.h"

namespace Utilities {
	std::vector<std::string> ParseCSV(const std::string& str) {
		std::vector<std::string> out{}; // Initialise output vector
		std::stringstream ss{ str }; // Make a stream out of str

		std::string s{}; // Receptacle for each string
		size_t quotes{ 0 }; // Count number of quotes in each string
		while (true) {
			char temp; // Receptacle for single character
			temp = ss.get(); // Next character from stream
			if(ss.eof()) { // Break condition is after getting the next character...
				out.push_back(s);
				break;
			}
			
			// The rules for valid CSV imply a quote is part of a string when it's not the first one, then every other
			if(temp == '"') {
				quotes++;
				if(quotes > 1 && quotes % 2 == 1) { 
					s += temp;
				}
			}
			// The rules for valid CSV imply a comma is part of a string when it has an odd number of quotes so far
			else if(temp == ',') {
				if(quotes % 2 == 1) { 
					s += temp;
				}
				else {
					out.push_back(s);
					s = "";
					quotes = 0;
				} 
			}
			else {
				s += temp;
			}
			
		}
		
		return out;
	}

	const std::string MakeCSV(const std::vector<std::string> &vec) {
		std::string out = "";
		for (auto &str: vec) {
			// parse string
			bool special = false; // the string contains a special character?
			std::string f_str{}; // string formatted for CSV
			for (auto &ch: str) {
				if (!special && (ch == '"' || ch == ',' || ch == '\n')) {
					special = true;
				}

				if (ch == '"') {
					f_str += "\"\""; /* quotes have to be escaped with another quote
									* (note other special characters aren't escaped) */
				}
				else {
					f_str += ch;
				}
			}

			// add string value to CSV
			if (special) {
				f_str = "\"" + f_str + "\""; // string with special characters has to be wrapped in quotes
			}
			out += f_str + ","; // comma separated
		}
		return out.substr(0, out.size()-1); // remove the final comma
	}

	bool EmptyCSV(const std::string &str) {
		return std::all_of(str.begin(), str.end(), [](char ch) {
			return ch == ',';
		});
	}

	std::unordered_map<std::string, std::vector<std::string>> ReadLists(const std::string& filename) {
		std::unordered_map<std::string, std::vector<std::string>> out{};

		// look for filename
		std::ifstream file{ filename };
		if(!file.good()) {
			throw std::runtime_error("data files not found. Please make sure you're running the program in the install directory.");
		}

		// process filename
		while (file.good()) {
			// heading
			std::string heading{};
			std::getline(file, heading); // puts the whole line in heading
			if(heading.empty()) {
				throw std::runtime_error("Invalid format of acquisition.txt or draws.txt -- expected HEADING, DATA, EMPTY LINE");
			}

			// comma-separated data
			std::string data{};
			std::getline(file, data); // puts the whole line in line
			if(data.empty()) {
				throw std::runtime_error("Invalid format of acquisition.txt or draws.txt -- expected HEADING, DATA, EMPTY LINE");
			}
			if (out.find(heading) != out.end()) {
				throw std::runtime_error("The heading names in acquisition.txt or draws.txt must not contain duplicates.");
			}
			out[heading] = ParseCSV(data); // adds the list to the hashmap

			// empty line
			std::getline(file, heading); // puts the whole line in heading, just temporarily
			if(!heading.empty()) {
				throw std::runtime_error("Invalid format of acquisition.txt or draws.txt -- expected HEADING, DATA, EMPTY LINE");
			}
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

	template<typename T> std::vector<T> Select(const std::vector<T> &vec, const std::function<bool(const T &)> &condition) {
		std::vector<T> out{};
		for (auto &item : vec) {
			if (condition(item)) {
				out.push_back(item);
			}
		}
		return out;
	}
	template std::vector<Hero> Select(const std::vector<Hero> &vec, const std::function<bool(const Hero &)> &condition);

	template<typename T, typename S> void Merge(std::unordered_map<T, S> &m1, const std::unordered_map<T, S> &m2) {
		for(auto& item: m2) { // item is {key: value}
			if (m1.find(item.first) != m1.end()) {
				throw std::runtime_error("The heading names in data/acquisition.txt and data/draws.txt must not contain duplicates.");
			}

			m1[item.first] = item.second;
		}
	}
	template void Merge(std::unordered_map<std::string, std::vector<std::string>> &acquisition, const std::unordered_map<std::string, std::vector<std::string>> &draws);

	void ValidateList(const std::vector<std::string> &data, size_t data_size, std::vector<size_t> numerical_data, std::vector<size_t> boolean_data) {
		std::string hero = data[0];
		if (data.size() != data_size) {
			std::string error_message = "Invalid format in data file. Hero " + hero + " has " + std::to_string(data.size()) + " items instead of " + std::to_string(data_size) + ".";
			throw std::runtime_error(error_message);
		}

		for(size_t i = 0; i < data_size; i++) {
			// check if it's a number
			try {
				std::stoi(data[i]); // is a number
				if(std::find(numerical_data.begin(), numerical_data.end(), i) == numerical_data.end()) { // not numerical data
					std::string error_message = "Invalid data in data/owned.csv. Item \"" + data[i] + "\" of hero " + hero + " should not be a number.";
					throw std::runtime_error(error_message);
				} else {
					// fine 
				}
			} catch (const std::invalid_argument &) { // not a number
				if(std::find(numerical_data.begin(), numerical_data.end(), i) != numerical_data.end()) { // is numerical data
					std::string error_message = "Invalid data in data/owned.csv. Item \"" + data[i] + "\" of hero " + hero + " should be a number.";
					throw std::runtime_error(error_message);
				} else {
					// fine 
				}
			}
			// check if it's a boolean
			if(data[i] == "true" || data[i] == "false") { // is a boolean
				if(std::find(boolean_data.begin(), boolean_data.end(), i) == boolean_data.end()) { // not boolean data
					std::string error_message = "Invalid data in data/owned.csv. Item \"" + data[i] + "\" of hero " + hero + " should not be a boolean.";
					throw std::runtime_error(error_message);
				} else {
					// fine 
				}
			} else { // not a boolean
				if(std::find(boolean_data.begin(), boolean_data.end(), i) != boolean_data.end()) { // is boolean data
					std::string error_message = "Invalid data in data/owned.csv. Item \"" + data[i] + "\" of hero " + hero + " should be a boolean.";
					throw std::runtime_error(error_message);
				} else {
					// fine 
				}
			}
		}
	}
}
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Utilities.h"

namespace Utilities {
std::vector<std::string> ParseCSV(const std::string &str) {
  std::vector<std::string> out{}; // Initialise output vector
  if (str.empty()) {
    return out;
  }

  std::stringstream ss{str}; // Make a stream out of str

  std::string s{};  // Receptacle for each string
  size_t quotes{0}; // Count number of quotes in each string
  while (true) {
    char temp{};                        // Receptacle for single character
    temp = static_cast<char>(ss.get()); // Next character from stream
    if (ss.eof()) { // Break condition is after getting the next character...
      out.push_back(s);
      break;
    }

    // The rules for valid CSV imply a quote is part of a string when it's not
    // the first one, then every other
    if (temp == '"') {
      quotes++;
      if (quotes > 1 && quotes % 2 == 1) {
        s += temp;
      }
    }
    // The rules for valid CSV imply a comma is part of a string when it has an
    // odd number of quotes so far
    else if (temp == ',') {
      if (quotes % 2 == 1) {
        s += temp;
      } else {
        out.push_back(s);
        s = "";
        quotes = 0;
      }
    } else {
      s += temp;
    }
  }

  return out;
}

std::string MakeCSV(const std::vector<std::string> &vec) {
  std::string out{};
  for (const auto &str : vec) {
    // parse string
    bool special = false; // the string contains a special character?
    std::string f_str{};  // string formatted for CSV
    for (const auto &ch : str) {
      if (!special && (ch == '"' || ch == ',' || ch == '\n')) {
        special = true;
      }

      if (ch == '"') {
        f_str += "\"\""; /* quotes have to be escaped with another quote
                          * (note other special characters aren't escaped) */
      } else {
        f_str += ch;
      }
    }

    // add string value to CSV
    if (special) {
      f_str = "\"" + f_str + "\""; // string with special characters has to be
                                   // wrapped in quotes NOLINT
    }
    out += f_str + ","; // comma separated
  }
  return out.substr(0, out.size() - 1); // remove the final comma
}

bool EmptyCSV(const std::string &str) {
  return std::all_of(str.begin(), str.end(), [](char ch) { return ch == ','; });
}

std::unordered_map<std::string, std::vector<std::string>>
ReadLists(const std::string &filename) {
  std::unordered_map<std::string, std::vector<std::string>> out{};

  // look for filename
  std::ifstream file{filename};
  if (!file.good()) {
    throw std::runtime_error("data files not found. Please make sure you're "
                             "running the program in the install directory.");
  }

  // process filename
  while (file.good()) {
    // heading
    std::string heading{};
    std::getline(file, heading); // puts the whole line in heading
    if (heading.empty()) {
      throw std::runtime_error("Invalid format of acquisition.txt or draws.txt "
                               "-- expected HEADING, DATA, EMPTY LINE");
    }

    // comma-separated data
    std::string data{};
    std::getline(file, data); // puts the whole line in line
    if (data.empty()) {
      throw std::runtime_error("Invalid format of acquisition.txt or draws.txt "
                               "-- expected HEADING, DATA, EMPTY LINE");
    }
    if (out.find(heading) != out.end()) {
      throw std::runtime_error("The heading names in acquisition.txt or "
                               "draws.txt must not contain duplicates.");
    }
    out[heading] = ParseCSV(data); // adds the list to the hashmap

    // empty line
    std::getline(file,
                 heading); // puts the whole line in heading, just temporarily
    if (!heading.empty()) {
      throw std::runtime_error("Invalid format of acquisition.txt or draws.txt "
                               "-- expected HEADING, DATA, EMPTY LINE");
    }
  }
  return out;
}
} // namespace Utilities
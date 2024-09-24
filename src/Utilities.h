/**
 * @file Utilities.h
 * @brief These are utility functions that in theory aren't unique to this project.
 */

#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Utilities {
    /**
     * @param &str A string containing a single valid CSV record.
     * @return a vector of string values
     * @see https://www.ietf.org/rfc/rfc4180.txt
    */
    std::vector<std::string> ParseCSV(const std::string& str);

    /** 
     * @param &vec A vector of string values.
     * @return a string containing a single valid CSV record
     * @see https://www.ietf.org/rfc/rfc4180.txt
     */
    const std::string MakeCSV(const std::vector<std::string> &vec);

    /** 
     * @param &str A string containing a single valid CSV record. 
     * @return whether the record is empty (i.e., contains nothing except any commas) */
    bool EmptyCSV(const std::string& str);

    /**
     * Reads data from a text file into a hashmap. The format of the file contents is expected to be: \n 
     * Heading \\n \n 
     * Comma-separated list \\n \n 
     * (Empty line) \\n \n 
     * The file isn't a valid CSV file, as the lines are expected to have different numbers of elements.
     * But each line is formatted as a valid CSV record (i.e., comma-separated
     * and using quotes for special characters, etc).
     * 
     * @param &filename A string, the name of the text file
     * @return a hashmap {heading => data, ...}.
     * @throw std::runtime_error if filename can't be opened
     * @throw std::runtime_error if file contents aren't as expected
     */
    std::unordered_map<std::string, std::vector<std::string>> ReadLists(const std::string& filename);

    /** @brief Returns the list of headings only from ReadLists' output, i.e. the hash's keys. */
    std::vector<std::string> HashKeys(std::unordered_map<std::string, std::vector<std::string>> already_read_lists);

    /** 
     * @brief Overload << for vectors using << operator for the elements, 
     * separated by commas. Does nothing if vec is empty. */
    template<typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec);

    /**
     * @brief This function takes a vector input and uses << operator for the elements,
     * each on a separate line. 
     */
    template<typename T> void PrintList(const std::vector<T> &vec);

    /** 
     * @brief Just uses copy_if to return a list. That is, given a vector and a condition,
     * returns a vector containing the items satisfying the condition. 
     */
    template<typename T> std::vector<T> Select(const std::vector<T> &vec, const std::function<bool(const T &)> &condition);

    /**
     * @brief Merges the two maps by modifying m1, i.e., adds the keys of m2 into m1.
     * Note if they have keys in common, m2's will overwrite m1's!
     */
    template<typename T, typename S> void Merge(std::unordered_map<T, S> &m1, const std::unordered_map<T, S> &m2);

    /**
     * @brief Checks the size and data types of a list of data. The parameters numerical_data and boolean_data are lists of indices. 
     * Throws an error if the data is different from what it should be.
     */
    void ValidateList(const std::vector<std::string> &data, size_t data_size, std::vector<size_t> numerical_data, std::vector<size_t> boolean_data);
}
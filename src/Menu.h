#pragma once

/**
 * @file Menu.h
 * @brief Implements menu and inputs.
 */

#include <functional>
#include <string>
#include <vector>

#include "Filter.h"
#include "Project.h"

namespace Menu {
/**
 * @brief MenuFunction lets me easily name and call main menu functions at
 * runtime.
 */
struct MenuFunction {
  std::string name;
  std::function<void()> function;
};

/**
 * @brief Hardcoded list of all main menu functions
 */
const std::vector<MenuFunction> menu_functions = {{"Filter", Menu_Filter},
                                                  {"AddOwned", Menu_AddOwned}};

/**
 * @brief An interactive menu to choose from the program's capabilities.
 */
void TopLevelMenu();

/**
 * @brief Accepts an integer input within a given range.
 * @param min The lower end of the range
 * @param max The upper end of the range
 * @return the chosen integer
 * @throw std::runtime_error if user input isn't an integer between min and max
 */
int GetIntInput(int min, int max);

/**
 * @brief Asks for a selection from a list of options.
 * @param prompt The prompt to be displayed
 * @param options The list of options
 * @param min_is_zero Whether the list should be numbered starting from 0
 * @return the integer selected
 * @see GetIntInput
 * @throw std::invalid_argument if options is empty
 */
size_t AskForInput(const std::string &prompt,
                   const std::vector<std::string> &options,
                   bool min_is_zero = false);

/**
 * @brief Accepts a yes or no input.
 * @param prompt The prompt to be displayed
 * @return a boolean true or false
 * @throw std::runtime_error if user input isn't yes/y/no/n
 */
bool YesOrNoInput(const std::string &question);

/**
 * @brief Reads a user input.
 * @param prompt The prompt to be displayed
 * @return a string taken from input
 */
std::string GetFreeInput(const std::string &prompt);
}; // namespace Menu
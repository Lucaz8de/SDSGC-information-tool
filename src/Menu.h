#pragma once
#include <functional>
#include <string>
#include <vector>

#include "Hero.h"

// Condition is the type of the input to select, so it is the return type of all my functions in Conditions.h.
typedef std::function<bool(const Hero &)> Condition;

class Menu {
public:
  // constants
  struct ConditionFunction { // ConditionFunction lets me easily name and call my conditions at runtime.
    std::string name;
    std::function<Condition(const std::vector<std::string> &)> function;
  };
  struct Operation { // Operation lets me easily name and call my operations at runtime.
    std::string name;
    std::function<Condition(const std::vector<Condition> &)> function;
    int arity;
  };
  struct MenuFunction { // MenuFunction lets me easily name and call my functions at runtime.
    std::string name;
    std::function<void()> function;
  };
  static const std::vector<ConditionFunction> conditions; // List of functions (Conditions.cpp) are just hardcoded.
  static const std::vector<Operation> operations; // List of functions (Conditions.cpp) are just hardcoded.
  static const std::vector<MenuFunction> functions; // List of functions (Project.cpp) are just hardcoded.
  
  // A static variable
  static bool validating; // Keep track of whether choosing to validate data files

  // A basic function to get an integer input in a range, and else crash.
  static int GetIntInput(int min, int max);

  // This function displays a prompt and the options and calls GetIntInput.
  static int AskForInput(std::string prompt, std::vector<std::string> options);

  // This function displays a prompt and reads a yes/no input. Throws an error if input is not yes/no.
  static bool YesOrNoInput(std::string question);

  // An interactive menu to choose from the program's capabilities.
  static void TopLevelMenu();

  // An interactive menu to choose the arguments for conditions that need them. 
  // (Right now this chooses count options from a single list.)
  static std::vector<std::string> AskForArguments(std::string name, std::vector<std::string> options, int count);

  // The function that handles deciding which conditions need which arguments.
  static std::vector<std::string> GetArguments(int int_input);

  // An interactive menu to choose one condition to filter.
  static const Condition GetCondition(std::vector<std::string> &filter_stack);

  // The function that asks for any other needed condition(s) and applies the operation to them.
  static const Condition HandleOperation(Condition &condition, Menu::Operation operation, std::vector<std::string> &filter_stack);

  // An interactive menu to choose an operation (if any) to apply to the condition.
  static const Condition GetOperation(Condition &condition, bool &sentinel_finished, std::vector<std::string> &filter_stack);
};
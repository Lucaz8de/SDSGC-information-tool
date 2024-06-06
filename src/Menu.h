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
  static const std::vector<ConditionFunction> conditions; // List of functions are just hardcoded.
  static const std::vector<Operation> operations; // List of functions are just hardcoded.
};

// A basic function to get an integer input in a range, and else crash.
int GetIntInput(int min, int max);

// An interactive menu to choose the arguments for conditions that need them.
std::vector<std::string> GetArguments(int int_input);

// An interactive menu to choose one condition to filter.
const Condition GetCondition();

// An interactive menu to choose an operation (if any) to apply to the condition.
const Condition GetOperation(Condition &condition, bool &sentinel_finished);

// The function that asks for any other needed condition(s) and applies the operation to them.
const Condition HandleOperation(Condition &condition, Menu::Operation operation);
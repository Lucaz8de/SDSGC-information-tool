#pragma once
#include <functional>
#include <string>
#include <vector>

#include "Hero.h"

typedef std::function<bool(const Hero &)> Condition;

class Menu {
public:
  // constants
  struct ConditionFunction {
    std::string name;
    std::function<Condition(const std::vector<std::string> &)> function;
  };
  struct Operation {
    std::string name;
    std::function<Condition(const std::vector<Condition> &)> function;
    int arity;
  };
  static const std::vector<ConditionFunction> conditions;
  static const std::vector<Operation> operations;
  static const std::vector<std::string> draws;
};

// A basic function to get an integer input in a range, and else crash.
int GetIntInput(int min, int max);

// An interactive menu to choose the arguments for conditions that need them.
std::vector<std::string> GetArguments(int int_input);

// An interactive menu to choose one condition to filter.
const Condition GetCondition();

int AskForOperation();

const Condition GetOperation(Condition &condition, bool &sentinel_finished);
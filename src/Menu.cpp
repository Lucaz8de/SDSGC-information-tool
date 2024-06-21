#include "Menu.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Conditions.h"
#include "Hero.h"
#include "Utilitiesig.h"

const std::vector<Menu::ConditionFunction> Menu::conditions{
  {"AvailableFromDraw", AvailableFromDraw},
  {"Owned", Owned},
  {"Good", Good},
  {"Upgraded", Upgraded},
  {"Acquirable", Acquirable}
};

const std::vector<Menu::Operation> Menu::operations{
  {"And", And, 2},
  {"Or", Or, 2},
  {"Not", Not, 1}
};

// A basic function to get an integer input in a range, and else crash.
int GetIntInput(int min, int max) {
	std::cout << "$ ";
	int int_input{ min - 1 };
	std::cin >> int_input;
	if (!std::cin.good() || int_input < min || int_input > max) {
		std::string error_message = "You must only enter a number between " + std::to_string(min) + " and " + std::to_string(max) + ".";
		throw std::runtime_error(error_message);
	}
	else {
		return int_input;
	}
}

// An interactive menu to choose the arguments for conditions that need them.
std::vector<std::string> GetArguments(int int_input) {
	std::vector<std::string> arguments;
	switch (int_input) {
	case 1:
	{
		std::vector<std::string> draws = HashKeys(ReadLists("../data/draws.txt"));
		int argument_input{ -1 };
		std::cout << "Please select the draw." << std::endl;
		std::cout << "The program will give up if you enter an invalid input, so please don't." << std::endl;
		for (size_t i{ 1 }; i <= draws.size(); i++) {
			std::cout << i << ". " << draws[i - 1] << std::endl;
		}
		argument_input = GetIntInput(1, draws.size());
		std::cout << std::endl;

		arguments.push_back(draws[argument_input - 1]);
		break;
	}
	// default: leaving the vector empty is fine
	}
	return arguments;
}

// An interactive menu to choose one condition to filter.
const Condition GetCondition(std::vector<std::string> &filter_stack) {
	int int_input{ -1 };
	std::cout << "You can filter the heroes by selecting conditions to search for from the menu." << std::endl;
	std::cout << "The program will give up if you enter an invalid input, so please don't." << std::endl;
	for (size_t i{ 1 }; i <= Menu::conditions.size(); i++) {
		std::cout << i << ". " << Menu::conditions[i - 1].name << std::endl;
	}
	int_input = GetIntInput(1, Menu::conditions.size());
	std::cout << std::endl;

	std::vector<std::string> arguments = GetArguments(int_input);

	filter_stack.push_back(Menu::conditions[int_input - 1].name);
	return Menu::conditions[int_input - 1].function(arguments);
}

const Condition HandleOperation(Condition &condition, Menu::Operation operation, std::vector<std::string> &filter_stack) {
	std::vector<Condition> conditions{ condition };
	filter_stack.push_back(operation.name);
	if (operation.arity > 1) {
		std::cout << "You must choose " << operation.arity - 1 << " other condition(s)." << std::endl;
		for (size_t i{ 0 }; i < operation.arity - 1; i++) {
			std::cout << i + 1 << ":" << std::endl;
			conditions.push_back(GetCondition(filter_stack));
		}
	}
	return operation.function(conditions);
}

const Condition GetOperation(Condition &condition, bool &sentinel_finished, std::vector<std::string> &filter_stack) {
	std::cout << "You can choose to use logical operations to combine conditions." << std::endl;
	std::cout << "You must say them in the order you want them done, though." << std::endl;
	std::cout << "The program will give up if you enter an invalid input, so please don't." << std::endl;
	std::cout << "0. No (print results)" << std::endl;
	for (size_t i{ 1 }; i <= Menu::operations.size(); i++) {
		std::cout << i << ". " << Menu::operations[i - 1].name << std::endl;
	}
	int int_input = GetIntInput(0, Menu::operations.size());
	std::cout << std::endl;

	if (int_input == 0) {
		sentinel_finished = true;
		return condition;
	}
	else {
		return HandleOperation(condition, Menu::operations[int_input - 1], filter_stack);
	}
}
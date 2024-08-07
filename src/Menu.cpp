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
  {"Acquirable", Acquirable},
  {"Race", Race}
};

const std::vector<Menu::Operation> Menu::operations{
  {"And", And, 2},
  {"Or", Or, 2},
  {"Not", Not, 1}
};

int Menu::GetIntInput(int min, int max) {
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

int Menu::AskForInput(std::string prompt, std::vector<std::string> options) {
	std::cout << prompt << std::endl;
	std::cout << "The program will give up if you enter an invalid input, so please don't." << std::endl;
	for (size_t i{ 1 }; i <= options.size(); i++) {
		std::cout << i << ". " << options[i - 1] << std::endl;
	}
	return GetIntInput(1, options.size());
}

std::vector<std::string> Menu::GetArguments(int int_input, bool validating) {
	std::vector<std::string> arguments;
	switch (int_input) {
	case 1:
	{
		std::string DATA_DIR = "../data";
		std::vector<std::string> draws = HashKeys(ReadLists(DATA_DIR + "/draws.txt", validating));
		int argument_input{ -1 };
		argument_input = AskForInput("Please select the draw.", draws);
		std::cout << std::endl;

		arguments.push_back(draws[argument_input - 1]);
		break;
	}
	case 6:
	{
		std::vector<std::string> races{"Demon", "Giant", "Fairy", "Goddess", "Unknown", "Human"};
		int argument_input{ -1 };
		argument_input = AskForInput("Please select the race.", races);
		std::cout << std::endl;

		arguments.push_back(races[argument_input - 1]);
		break;
	}
	// default: leaving the vector empty is fine
	}
	return arguments;
}

const Condition Menu::GetCondition(std::vector<std::string> &filter_stack, bool validating) {
	int int_input{ -1 };
	std::vector<std::string> condition_names{};
	for(ConditionFunction condition: conditions) {
		condition_names.push_back(condition.name);
	}
	int_input = AskForInput("You can filter the heroes by selecting conditions to search for from the menu.", condition_names);
	std::cout << std::endl;

	std::vector<std::string> arguments = GetArguments(int_input, validating);

	filter_stack.push_back(conditions[int_input - 1].name);
	return conditions[int_input - 1].function(arguments);
}

const Condition Menu::HandleOperation(Condition &condition, Operation operation, std::vector<std::string> &filter_stack, bool validating) {
	std::vector<Condition> conditions{ condition };
	filter_stack.push_back(operation.name);
	if (operation.arity > 1) {
		std::cout << "You must choose " << operation.arity - 1 << " other condition(s)." << std::endl;
		for (size_t i{ 0 }; i < operation.arity - 1; i++) {
			std::cout << i + 1 << ":" << std::endl;
			conditions.push_back(GetCondition(filter_stack, validating));
		}
	}
	return operation.function(conditions);
}

const Condition Menu::GetOperation(Condition &condition, bool &sentinel_finished, std::vector<std::string> &filter_stack, bool validating) {
	// special input with 0 an option so don't use AskForInput
	std::cout << "You can choose to use logical operations to combine conditions." << std::endl;
	std::cout << "You must say them in the order you want them done." << std::endl;
	std::cout << "The program will give up if you enter an invalid input, so please don't." << std::endl;
	std::cout << "0. No (print results)" << std::endl;
	for (size_t i{ 1 }; i <= operations.size(); i++) {
		std::cout << i << ". " << operations[i - 1].name << std::endl;
	}
	int int_input = GetIntInput(0, operations.size());
	std::cout << std::endl;

	if (int_input == 0) {
		sentinel_finished = true;
		return condition;
	}
	else {
		return HandleOperation(condition, operations[int_input - 1], filter_stack, validating);
	}
}
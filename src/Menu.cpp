#include "Menu.h"

#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "Conditions.h"
#include "Hero.h"
#include "Utilitiesig.h"
#include "Project.h"

const std::vector<Menu::ConditionFunction> Menu::conditions{
  {"Attribute", Attribute},
  {"StartingGrade", StartingGrade},
  {"Character", Character},
  {"LR", LR},
  {"Race", Race},
  {"Characteristic", Characteristic},
  {"Good", Good},
  {"VeryGood", VeryGood},
  {"Best", Best},
  {"Owned", Owned},
  {"Acquirable", Acquirable},
  {"AvailableFromDraw", AvailableFromDraw},
  {"Upgraded", Upgraded},  
};

const std::vector<Menu::Operation> Menu::operations{
  {"And", And, 2},
  {"Or", Or, 2},
  {"Not", Not, 1}
};

const std::vector<Menu::MenuFunction> Menu::functions{
  {"Filter", Filter},
  {"AddOwned", AddOwned}
};

bool Menu::validating = false;

int Menu::GetIntInput(int min, int max) {
	std::cout << "$ ";
	std::string input;
	int int_input{ min - 1 };

	std::getline(std::cin, input);
	try {
		int_input = std::stoi(input);
		if (int_input < min || int_input > max) {
			throw std::invalid_argument("");
		}
	} 
	catch(std::invalid_argument) {
		std::string error_message = "You must only enter a number between " + std::to_string(min) + " and " + std::to_string(max) + ".";
		throw std::runtime_error(error_message);
	}
	
	return int_input;
}

int Menu::AskForInput(std::string prompt, std::vector<std::string> options) {
	std::cout << prompt << std::endl;
	for (size_t i{ 1 }; i <= options.size(); i++) {
		std::cout << i << ". " << options[i - 1] << std::endl;
	}
	return GetIntInput(1, options.size());
}

bool Menu::YesOrNoInput(std::string question) {
	std::string input;
	std::cout << question << " Enter yes/no." << std::endl;
	std::getline(std::cin, input);
	if(input == "y" || input == "Y" || input == "yes" || input == "Yes" || input == "YES") {
		return true;
	} else if(input == "n" || input == "N" || input == "no" || input == "No" || input == "NO") {
		return false;
	} else {
		throw std::runtime_error("Input " + input + " is not valid.");
	}
}

std::string Menu::GetFreeInput(std::string prompt) {
    std::string input;
	std::cout << prompt << ": ";
	std::getline(std::cin, input);
	std::cout << std::endl;
	return input;
}

void Menu::TopLevelMenu() {
	std::vector<std::string> function_names{};
	for(MenuFunction function: functions) {
		function_names.push_back(function.name);
	}
	int int_input = AskForInput("You can choose from the following capabilities.", function_names);
	std::cout << std::endl;
	functions[int_input - 1].function();
}

std::vector<std::string> Menu::AskForArguments(std::string name, std::vector<std::string> options, int count) {
	std::vector<std::string> arguments;

	for(int i = 0; i < count; i++) {
		int int_input = Menu::AskForInput("Please select the " + name + ".", options);
		std::cout << std::endl;
		arguments.push_back(options[int_input - 1]);
	}

	return arguments;
}

std::vector<std::string> Menu::GetArguments(int int_input) {
	switch (int_input) {
	case 1:
	{
		std::vector<std::string> attributes{"Strength", "HP", "Speed", "Light", "Darkness"};
		return AskForArguments("attribute", attributes, 1);
	}
	case 2:
	{
		std::vector<std::string> starting_grades{"R", "SR", "SSR", "UR"};
		return AskForArguments("starting grade", starting_grades, 1);
	}
	case 3:
	{
		return {GetFreeInput("Please enter a CORRECT character name, e.g. Meliodas")};
	}
	case 5:
	{
		std::vector<std::string> races{"Demon", "Giant", "Fairy", "Goddess", "Unknown", "Human"};
		return AskForArguments("race", races, 1);
	}
	case 6:
	{
		std::vector<std::string> characteristics{
			"The Seven Deadly Sins", "The Four Archangels", "The Ten Commandments", "Ragnarok", "The Seven Catastrophes", "Collab"
		};
		return AskForArguments("characteristic", characteristics, 1);
	}
	case 12:
	{
		std::string DATA_DIR = "../data";
		std::vector<std::string> draws = HashKeys(ReadLists(DATA_DIR + "/draws.txt", false));
		return AskForArguments("draw", draws, 1);
	}
	default:
		return {};
	}
}

const Condition Menu::GetCondition(std::stack<std::string> &filters) {
	int int_input{ -1 };
	std::vector<std::string> condition_names{};
	for(ConditionFunction condition: conditions) {
		condition_names.push_back(condition.name);
	}
	int_input = AskForInput("You can filter heroes by the following conditions.", condition_names);
	std::cout << std::endl;

	std::vector<std::string> arguments = GetArguments(int_input);

	filters.push(conditions[int_input - 1].name);
	return conditions[int_input - 1].function(arguments);
}

const Condition Menu::HandleOperation(Condition &condition, Operation operation, std::stack<std::string> &filters) {
	std::vector<Condition> conditions{ condition };
	filters.push(operation.name);
	if (operation.arity > 1) {
		std::cout << "You must choose " << operation.arity - 1 << " other condition(s)." << std::endl;
		for (size_t i{ 0 }; i < operation.arity - 1; i++) {
			std::cout << i + 1 << ":" << std::endl;
			conditions.push_back(GetCondition(filters));
		}
	}
	return operation.function(conditions);
}

const Condition Menu::GetOperation(Condition &condition, bool &sentinel_finished, std::stack<std::string> &filters) {
	// special input with 0 an option so don't use AskForInput
	std::cout << "You can choose to use logical operations to combine conditions." << std::endl;
	std::cout << "You must say them in the order you want them done." << std::endl;
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
		return HandleOperation(condition, operations[int_input - 1], filters);
	}
}
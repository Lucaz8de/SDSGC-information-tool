#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "Filter.h"
#include "Hero.h"
#include "Heroes.h"
#include "Menu.h"
#include "Utilities.h"

namespace Filter {
	std::vector<std::array<Hero, Heroes::COUNT>::iterator> Filter() {
		// initialise queue of conditions and get initial condition from user
		std::queue<std::string> filters{};
		Condition condition = GetCondition(filters);

		// applies as many additional operations (e.g. AND) as they want
		bool sentinel_finished = false;
		while (!sentinel_finished) {
			condition = GetOperation(condition, sentinel_finished, filters);
		}

		// print the final condition (strictly left-to-right) for reference
		std::cout << "You have selected: ";
		for(size_t i{ 0 }; i < filters.size(); i++) {
			std::cout << "(";
		}
		while(!filters.empty()) {
			std::cout << filters.front() << ") ";
			filters.pop();
		}

		// return the final filtered list
		auto filtered = Utilities::Select(Heroes::list, condition);
		std::cout << "\nFound: " << filtered.size() << " out of " << Heroes::list.size() <<
			" heroes." << std::endl;
		return filtered;
	}

	Condition GetCondition(std::queue<std::string> &filters) {
		// Ask for a condition
		size_t int_input{ 0 };
		std::vector<std::string> condition_names{"All"};
		for(const ConditionFunction& condition: conditions) {
			condition_names.push_back(condition.name);
		}
		int_input = Menu::AskForInput("You can filter heroes by the following conditions.", condition_names, true);
		std::cout << std::endl;

		// Return unconditional if asked for
		if (int_input == 0) {
			filters.emplace("ALL");
			return [](const Hero &hero) { return true; };
		}
		else {
			// Ask for arguments if needed, push condition to queue and return it
			std::vector<std::string> arguments = GetArguments(int_input);
			filters.push(conditions[int_input - 1].name);
			return conditions[int_input - 1].function(arguments);
		}
	}

	std::vector<std::string> GetArguments(size_t int_input) {
		const ConditionFunction& condition = conditions[int_input-1];
		if (condition.name == "Attribute") {
			std::vector<std::string> attributes{"Strength", "HP", "Speed", "Light", "Darkness"};
			return AskForArguments("attribute", attributes, 1);
		}
		else if (condition.name == "StartingGrade") {
			std::vector<std::string> starting_grades{"R", "SR", "SSR", "UR"};
			return AskForArguments("starting grade", starting_grades, 1);
		}
		else if (condition.name == "Character") {
			return {Menu::GetFreeInput("Please enter a CORRECT character name, e.g. Meliodas")};
		}
		else if (condition.name == "Race") {
			std::vector<std::string> races{"Demon", "Giant", "Fairy", "Goddess", "Unknown", "Human"};
			return AskForArguments("race", races, 1);
		}
		else if (condition.name == "Characteristic") {
			std::vector<std::string> characteristics{
				"The Seven Deadly Sins", "Four Knights of the Apocalypse", "The Four Archangels", "The Ten Commandments", "Ragnarok", "The Seven Catastrophes", "Collab"
			};
			return AskForArguments("characteristic", characteristics, 1);
		}
		else if (condition.name == "Good") {
			size_t count_input = Menu::AskForInput("Please select the NUMBER of tiers.", {"1", "2", "3", "4", "5"});
			std::cout << std::endl;
			std::vector<std::string> tiers{"1", "2", "3", "4", "5"};
			return AskForArguments("tiers", tiers, count_input);
		}
		else if (condition.name == "AvailableFromDraw") {
			std::string DATA_DIR = "../data";
			std::vector<std::string> draws = Utilities::HashKeys(Utilities::ReadLists(DATA_DIR + "/draws.txt"));
			return AskForArguments("draw", draws, 1);
		}
		else if (condition.name == "Upgraded") {
			std::cout << "Enter the minimum level of heroes you want to show." << std::endl;
			int int_input = Menu::GetIntInput(1, 100);
			std::cout << std::endl;
			return {std::to_string(int_input)};
		}
		else {
			return {};
		}
	}

	std::vector<std::string> AskForArguments(std::string name, std::vector<std::string> options, size_t count) {
		std::vector<std::string> arguments(count);

		std::string prompt;
		// ask for count choices and add them to list of arguments
		for(size_t i = 0; i < count; i++) {
			prompt = "Please select the " + name;
			prompt += " (" + std::to_string(count-i) + " more).";
			size_t int_input = Menu::AskForInput(prompt, options);
			std::cout << std::endl;
			arguments[i] = options[int_input - 1];
		}

		return arguments;
	}

	Condition GetOperation(Condition &condition, bool &sentinel_finished, std::queue<std::string> &filters) {
		// Make list of no + operation names
		std::vector<std::string> options = {"No (get results)"};
		for(const auto &operation: operations) {
			options.push_back(operation.name);
		}

		// Ask for selection from list
		size_t int_input = Menu::AskForInput(
			"You can choose to use logical operations to combine conditions.\n"
			"You must say them in the ORDER you want them done.",
			options,
			true
		);
		std::cout << std::endl;

		// Handle the selection
		if (int_input == 0) {
			sentinel_finished = true;
			return condition;
		}
		else {
			const Operation& chosen = operations[int_input - 1];
			return HandleOperation(condition, chosen, filters);
		}
	}

	Condition HandleOperation(Condition &condition, Operation operation, std::queue<std::string> &filters) {
		if (operation.arity <= 0) {
			throw std::invalid_argument("Invalid operation, must have positive 'arity'");
		}
		// initialise list of conditions to be operated on
		std::vector<Condition> conditions(operation.arity);

		// add operation to filter queue
		filters.push(operation.name);

		// get all required conditions for operation
		conditions[0] = condition;
		if (operation.arity > 1) {
			std::cout << "You must choose " << operation.arity - 1 << " other condition(s)." << std::endl;
			for (size_t i{ 1 }; i <= operation.arity - 1; i++) {
				std::cout << i << ":" << std::endl;
				conditions[i] = GetCondition(filters);
			}
		}

		// return result of operation
		return operation.function(conditions);
	}

	// conditions 

	Condition Attribute(const std::vector<std::string> &arguments) {
		if (arguments.size() != 1) {
			throw std::invalid_argument("Input to Attribute should be 1 string");
		}
		const std::string& attribute = arguments.at(0);
		return [attribute](const Hero &hero) -> bool {
			return hero.attribute == attribute;
		};
	}

	Condition StartingGrade(const std::vector<std::string> &arguments) {
		if (arguments.size() != 1) {
			throw std::invalid_argument("Input to StartingGrade should be 1 string");
		}
		const std::string& grade = arguments.at(0);
		return [grade](const Hero &hero) -> bool {
			return hero.starting_grade == grade;
		};
	}

	Condition Character(const std::vector<std::string> &arguments) {
		if (arguments.size() != 1) {
			throw std::invalid_argument("Input to Character should be 1 string");
		}
		const std::string& character = arguments.at(0);
		return [character](const Hero &hero) -> bool {
			return hero.character == character;
		};
	}

	Condition LR(const std::vector<std::string> &arguments) {
		if (arguments.size() != 0) {
			throw std::invalid_argument("Input to LR should be empty");
		}
		return [](const Hero &hero) -> bool {
			return hero.lr;
		};
	}

	Condition Race(const std::vector<std::string> &arguments) {
		if (arguments.size() != 1) {
			throw std::invalid_argument("Input to Race should be 1 string");
		}
		const std::string& race = arguments.at(0);
		return [race](const Hero &hero) -> bool {
			return std::find(hero.races.begin(), hero.races.end(), race) != hero.races.end();
		};
	}

	Condition Characteristic(const std::vector<std::string> &arguments) {
		if (arguments.size() != 1) {
			throw std::invalid_argument("Input to Characteristic should be 1 string");
		}
		const std::string& characteristic = arguments.at(0);
		return [characteristic](const Hero &hero) -> bool {
			return hero.characteristic == characteristic;
		};
	}

	Condition Good(const std::vector<std::string> &arguments) {
		if (arguments.size() == 0) {
			throw std::invalid_argument("Input to Good should be at least 1 string");
		}
		const std::vector<std::string>& input_tiers = arguments;
		return [input_tiers](const Hero &hero) -> bool {
			return std::any_of(hero.tiers.begin(), hero.tiers.end(), [input_tiers](int tier) {
				return std::find(input_tiers.begin(), input_tiers.end(), std::to_string(tier)) != input_tiers.end();
			});
		};
	}

	Condition AvailableFromDraw(const std::vector<std::string> &arguments) {
		if (arguments.size() != 1) {
			throw std::invalid_argument("Input to AvailableFromDraw should be 1 string");
		}
		const std::string& draw = arguments.at(0);
		return [draw](const Hero &hero) -> bool {
			std::vector<std::string> acquisition = hero.acquisition;
			return std::find(acquisition.begin(), acquisition.end(), draw) != acquisition.end();
		};
	}

	Condition Owned(const std::vector<std::string> &arguments) {
		if (arguments.size() != 0) {
			throw std::invalid_argument("Input to Owned should be empty");
		}
		return [](const Hero &hero) -> bool {
			return hero.owned;
		};
	}

	Condition Upgraded(const std::vector<std::string> &arguments) {
		if (arguments.size() != 1) {
			throw std::invalid_argument("Input to Upgraded should be 1 string");
		}
		int level = std::stoi(arguments.at(0));
		return [level](const Hero &hero) -> bool {
			return hero.owned && (hero.upgrades[Hero::GRADE] == "LR" || std::stoi(hero.upgrades[Hero::LEVEL]) >= level);
		};
	}

	Condition Acquirable(const std::vector<std::string> &arguments) {
		if (arguments.size() != 0) {
			throw std::invalid_argument("Input to Acquirable should be empty");
		}
		return [](const Hero &hero) -> bool {
			return !(hero.acquisition.empty());
		};
	}

	// operations

	Condition And(const std::vector<Condition> &conditions) {
		if (conditions.size() != 2) {
			throw std::invalid_argument("Input to OR should be 2 conditions");
		}
		Condition A{conditions.at(0)};
		Condition B{conditions.at(1)};
		return [A, B](const Hero &hero) -> bool {
			return A(hero) && B(hero);
		};
	}

	Condition Or(const std::vector<Condition> &conditions) {
		if (conditions.size() != 2) {
			throw std::invalid_argument("Input to OR should be 2 conditions");
		}
		Condition A{conditions.at(0)};
		Condition B{conditions.at(1)};
		return [A, B](const Hero &hero) -> bool {
			return A(hero) || B(hero);
		};
	}

	Condition Not(const std::vector<Condition> &conditions) {
		if (conditions.size() != 1) {
			throw std::invalid_argument("Input to NOT should be 1 condition");
		}
		Condition A{conditions.at(0)};
		return [A](const Hero &hero) -> bool {
			return !A(hero);
		};
	}
}
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "Hero.h"
#include "Menu.h"
#include "Utilities.h"
#include "Project.h"

namespace Menu {
	void TopLevelMenu() {
		std::vector<std::string> function_names{};
		for(MenuFunction function: menu_functions) {
			function_names.push_back(function.name);
		}
		
		size_t int_input = AskForInput("You can choose from the following capabilities.", function_names);
		std::cout << std::endl;
		menu_functions[int_input - 1].function();
	}

	int GetIntInput(int min, int max) {
		std::string input;
		int int_input{ min - 1 };

		// prompt for input
		std::cout << "$ ";
		std::getline(std::cin, input);

		// validate input
		try {
			int_input = std::stoi(input);
			if (int_input < min || int_input > max) {
				throw std::runtime_error("");
			}
		} 
		catch(const std::runtime_error &) {
			std::string error_message = "You must only enter a number between " + std::to_string(min) + " and " + std::to_string(max) + ".";
			throw std::runtime_error(error_message);
		}
		catch(const std::invalid_argument &) { // std::stoi couldn't convert
			std::string error_message = "You must enter a number between " + std::to_string(min) + " and " + std::to_string(max) + ".";
			throw std::runtime_error(error_message);
		}
		
		return int_input;
	}

	size_t AskForInput(std::string prompt, std::vector<std::string> options, bool min_is_zero) {
		if (options.empty()) {
			throw std::invalid_argument("Options for AskForInput cannot be empty");
		}

		// determine how the list should be numbered
		size_t min = 1 - static_cast<int>(min_is_zero); // think about it
		size_t max = options.size() - static_cast<int>(min_is_zero); // think about it
		
		std::cout << prompt << std::endl;

		// print the list numbered correctly 
		size_t option_number{ min };
		for (std::string option : options) {
			std::cout << option_number++ << ". " << option << std::endl;
		} 
		
		return static_cast<size_t>(GetIntInput(static_cast<int>(min), static_cast<int>(max)));
	}

	bool YesOrNoInput(std::string question) {
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

	std::string GetFreeInput(std::string prompt) {
		std::string input;

		std::cout << prompt << ": ";
		std::getline(std::cin, input);
		std::cout << std::endl;
		return input;
	}
}
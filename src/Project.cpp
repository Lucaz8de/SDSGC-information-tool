#include <fstream>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "AddOwned.h"
#include "Conditions.h"
#include "Hero.h"
#include "Menu.h"
#include "Project.h"
#include "Utilitiesig.h"

int main() {
	// Option to validate data files
	Menu::validating = Menu::YesOrNoInput("You must validate your data. Please only say no if you have already done so. Validate?");

	// Read data files and generate list of heroes
	Hero::MakeHeroes();
	if(Menu::validating) {
		// When a data file is deemed to be invalid, the program will end with a helpful error message.
		std::cout << "Data files ok" << std::endl;
	}
	std::cout << std::endl;

	// Run the program loop. This allows the user to use the program until they close it.
	while (true) {
		Menu::TopLevelMenu();
		std::cout << std::endl << std::endl;
	}

	return 0;
}

void Filter() {
	// initialise stack of filters and get initial condition from user
	std::stack<std::string> filters{};
	Condition condition = Menu::GetCondition(filters);

	// applies as many additional operations (e.g. AND) as they want
	bool sentinel_finished = false;
	while (!sentinel_finished) {
		condition = Menu::GetOperation(condition, sentinel_finished, filters);
	}

	// print the final condition (strictly left-to-right) for reference
	std::cout << "You have selected: ";
	for(size_t i{ 0 }; i < filters.size(); i++) {
		std::cout << "(";
	}
	while(!filters.empty()) {
		std::cout << filters.top() << ") ";
		filters.pop();
	}

	// filter and print the final filtered list
	auto found = Select(Hero::heroes, condition);
	std::cout << "\nFound: " << found.size() << " out of " << Hero::heroes.size() <<
		" heroes. Press enter to view results." << std::endl;
	std::string x{};
	std::getline(std::cin, x);
	PrintList(Select(Hero::heroes, condition));
}

void AddOwned() {
	if (!Menu::validating) {
		throw std::runtime_error("To edit data files, validation is required. Please re-run the program and type yes.");
	}

	std::vector<std::string> options = {
		"Select from unowned heroes list to add new owned hero.",
		"Select from owned heroes list to upgrade owned hero.",
		"Select from full heroes list to update owned hero list.",
		"Search by name."
	};
	int option = Menu::AskForInput("Please choose from the following menu how you'd like to add heroes.", options);

	if (option < options.size()) { // Only the last option asks for input.
		UpdateHeroes(option);
	}
	else {
		std::string character;
		std::cout << "Please enter a CORRECT character name, e.g. Meliodas: ";
		std::getline(std::cin, character);
		UpdateHeroes(character);
	}

	UpdateFile();
	std::cout << "data/owned.csv file updated." << std::endl;
}
#include <fstream>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "Hero.h"
#include "Menu.h"
#include "Utilitiesig.h"
#include "Project.h"
#include "AddOwned.h"

typedef std::function<bool(const Hero &)> Condition;

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

	while (true) {
		Menu::TopLevelMenu();
		std::cout << std::endl << std::endl;
	}

	return 0;
}

void Filter() {
	std::vector<Hero> heroes = Hero::heroes;

	std::stack<std::string> filters{};
	bool sentinel_finished = false;
	Condition condition = Menu::GetCondition(filters);
	while (!sentinel_finished) {
		condition = Menu::GetOperation(condition, sentinel_finished, filters);
	}

	std::string x{};
	std::cout << "You have selected: ";
	for(size_t i{ 0 }; i < filters.size(); i++) {
		std::cout << "(";
	}
	while(!filters.empty()) {
		std::cout << filters.top() << ") ";
		filters.pop();
	}
	auto found = Select(heroes, condition);
	std::cout << "\nFound: " << found.size() << " out of " << heroes.size() <<
		" heroes. Press enter to view results." << std::endl;
	std::getline(std::cin, x);

	PrintList(Select(heroes, condition));
}

void AddOwned() {
	if (!Menu::validating) {
		throw std::runtime_error("To edit data files, validation is required. Please re-run the program and type yes.");
	}

	int option = Menu::AskForInput(
		"Please choose from the following menu how you'd like to add heroes.",
		{
			"Select from unowned heroes list to add new owned hero.",
			"Select from owned heroes list to upgrade owned hero.",
			"Select from full heroes list to update owned hero list.",
			"Search by name."
		}
	);

	if (option < 3) {
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
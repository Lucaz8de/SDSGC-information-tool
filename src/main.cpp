#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "AddOwned.h"
#include "Filter.h"
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

void Menu_Filter() {
	// print the filtered list
	std::vector<Hero> filtered = Filter::Filter();
	std::cout << "Press enter to view results." << std::endl;
	std::string x{};
	std::getline(std::cin, x);
	PrintList(filtered);
}

void Menu_AddOwned() {
	if (!Menu::validating) {
		throw std::runtime_error("To edit data files, validation is required. Please re-run the program and type yes.");
	}

	std::cout << "Please select the heroes you want to update." << std::endl;
	std::vector<Hero> heroes = Filter::Filter();

	AddOwned::UpdateHeroes(heroes);

	AddOwned::UpdateFile();
	std::cout << "data/owned.csv file updated." << std::endl;
}
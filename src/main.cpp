#include <iostream>
#include <string>
#include <vector>

#include "AddOwned.h"
#include "Filter.h"
#include "Hero.h"
#include "Heroes.h"
#include "Menu.h"
#include "Project.h"

int main() {
	// Read data files and generate list of heroes
	try {
		Heroes::MakeHeroes();
	}
	catch (std::exception &ex) {
		// should make custom exception types
		// e.g. exceptions are thrown when a data file isn't found or has invalid data
		// these errors aren't recoverable, i.e. the data needs to be changed before
		// the program can retry.
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Data files ok" << std::endl;
	std::cout << std::endl;

	// Run the program loop. This allows the user to use the program until they close it.
	std::string x{};
	while (true) {
		Menu::TopLevelMenu();
		std::cout << std::endl << std::endl;
		std::cout << "Press x to exit, anything else to continue. " << std::endl;
		std::getline(std::cin, x);
		if (x == "x") {
			return EXIT_SUCCESS;
		}
	}
}

void Menu_Filter() {
	std::vector<std::array<Hero, Heroes::COUNT>::iterator> filtered = Filter::Filter();

	std::cout << "Press enter to view results." << std::endl;
	std::string x{};
	std::getline(std::cin, x);

	for (auto &it : filtered) {
		std::cout << *it << std::endl;
	}
}

void Menu_AddOwned() {
	std::cout << "Please select the heroes you want to update." << std::endl;
	std::vector<std::array<Hero, Heroes::COUNT>::iterator> heroes = Filter::Filter();

	AddOwned::UpdateHeroes(heroes);

	AddOwned::UpdateFile();
	std::cout << "data/owned.csv file updated." << std::endl;
}
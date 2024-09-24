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
#include "Utilities.h"

int main() {
	// Read data files and generate list of heroes
	Hero::MakeHeroes();
	/* When a data file is found to be invalid, an exception is thrown with a helpful error message.
	 * This error isn't recoverable, i.e. the data needs to be changed and the program needs to be rerun.
	 * So it exits. Otherwise, it continues. */

	std::cout << "Data files ok" << std::endl;
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
	Utilities::PrintList(filtered);
}

void Menu_AddOwned() {
	std::cout << "Please select the heroes you want to update." << std::endl;
	std::vector<Hero> heroes = Filter::Filter();

	AddOwned::UpdateHeroes(heroes);

	AddOwned::UpdateFile();
	std::cout << "data/owned.csv file updated." << std::endl;
}
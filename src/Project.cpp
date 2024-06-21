#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Hero.h"
#include "Menu.h"
#include "Utilitiesig.h"

typedef std::function<bool(const Hero &)> Condition;

int main() {
	Hero::MakeHeroes(); // Set up the list of heroes
	std::vector<Hero> heroes = Hero::get_heroes();

	std::vector<std::string> filter_stack{};
	bool sentinel_finished = false;
	Condition condition = GetCondition(filter_stack);
	while (!sentinel_finished) {
		condition = GetOperation(condition, sentinel_finished, filter_stack);
	}

	std::string x{};
	std::cout << "You have selected: ";
	for(std::string &word: filter_stack) {
		std::cout << "(";
	}
	for(std::string &word: filter_stack) {
		std::cout << word << ") ";
	}
	std::cout << "\nType anything to view results." << std::endl;
	std::cin >> x;

	PrintList(Select(heroes, condition));

	return 0;
}
#include <cstdlib>
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

	bool sentinel_finished = false;
	Condition condition = GetCondition();
	while (!sentinel_finished) {
		condition = GetOperation(condition, sentinel_finished);
	}
	PrintList(select(heroes, condition));

	return 0;
}

static void todo() {
	std::cout << "There is a bug ... Using 'AND' just returns every hero ..." << std::endl;
	std::cout << "Print upgrades and upgrade costs" << std::endl;
	std::cout << "Add a higher level of the menu asking what you want to do," << 
		"e.g. filter, print all upgrade costs ..." << std::endl;
}
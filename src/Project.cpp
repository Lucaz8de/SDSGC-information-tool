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

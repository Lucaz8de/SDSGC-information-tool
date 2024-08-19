#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "AddOwned.h"
#include "Hero.h"
#include "Menu.h"
#include "Utilitiesig.h"

void UpdateFile() {
    std::string DATA_DIR = "../data";
    std::ofstream file{ DATA_DIR + "/owned.csv" };

	for(auto &hero: Hero::heroes) {
		if(!hero.get_owned()) {
			continue;
		}

		std::vector<std::string> line_vec{hero.upgrades};
		line_vec.insert(line_vec.begin(), hero.get_hero());
		std::string csv_string = MakeCSV(line_vec);
		file << csv_string << "\n";
	}
}

void UpdateHero(Hero &hero) {
    if (!hero.get_owned()) {
        hero.owned = true;
        hero.upgrades = std::vector<std::string>(5, "");
    }

	int input;
	std::vector<std::string> grades{ "R", "SR", "SSR", "UR", "LR" };
	input = Menu::AskForInput("Please select hero's grade.", grades);
	hero.upgrades[Hero::GRADE] = grades[input-1];

	std::vector<std::string> levels{
		"30", "40", "50", "60", "65", "70", "75", "80", "85", "90", "95", "100"
	};
	input = Menu::AskForInput("Please select hero's maximum level.", levels);
	hero.upgrades[Hero::LEVEL] = levels[input-1];

	std::vector<std::string> stars{
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"
	};
	input = Menu::AskForInput("Please select hero's awakening stars.", stars);
	hero.upgrades[Hero::STARS] = stars[input-1];

	std::vector<std::string> uniques{"true", "false"};
	input = Menu::AskForInput("Please select whether hero has unlocked their unique ability.", uniques);
	hero.upgrades[Hero::UNIQUE] = uniques[input-1];

	std::vector<std::string> ultimates{"1", "2", "3", "4", "5", "6"};
	input = Menu::AskForInput("Please select the hero's ultimate move level.", ultimates);
	hero.upgrades[Hero::ULTIMATE] = ultimates[input-1];
}

void UpdateHeroes(std::string character) {
    std::vector<Hero> list = Hero::heroes;
    Condition is_character = [character](const Hero &hero) -> bool {
        return hero.get_character() == character;
    };

    UpdateHeroes(3, Select(list, is_character));
}

void UpdateHeroes(int option, std::vector<Hero> list) {
	for(auto& hero: Hero::heroes) {
		if (hero.get_owned() && option == 1) {
			continue;
		}
		if (!hero.get_owned() && option == 2) {
			continue;
		}
        if (!list.empty()) {
            bool found = false;
            for (auto& search_hero: list) {
                if (hero.get_hero() == search_hero.get_hero()) {
                    found = true;
                }
            }
            if (!found) {
                continue;
            }
        }

		std::cout << hero.get_hero() << ":" << std::endl;
		bool update;
		if (hero.get_owned()) {
			// print upgrades
			update = Menu::YesOrNoInput("Want to update this hero?");
		}
		else {
			std::cout << "Not owned yet." << std::endl;
			update = Menu::YesOrNoInput("Want to add to owned?");
		}
		if (update) {
			UpdateHero(hero);
		}
	}
}
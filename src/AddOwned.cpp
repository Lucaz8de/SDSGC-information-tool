#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "AddOwned.h"
#include "Hero.h"
#include "Heroes.h"
#include "Menu.h"
#include "Utilities.h"

namespace AddOwned {
void UpdateFile() {
  // open heroes.csv and owned.csv data files
  std::string DATA_DIR = "../data";
  std::ifstream ifile{DATA_DIR + "/heroes.csv"};
  std::ofstream ofile{DATA_DIR + "/owned.csv"};

  std::string line;
  for (auto &hero : Heroes::list) {
    std::getline(ifile, line);
    // add empty lines in the places heroes.csv has empty lines
    if (Utilities::EmptyCSV(line)) {
      ofile << ",,,,,\n";
      std::getline(ifile, line);
    }

    // add data about owned heroes
    if (!hero.owned) {
      continue;
    }
    std::vector<std::string> vec_data{hero.upgrades.begin(),
                                      hero.upgrades.end()};
    vec_data.insert(vec_data.begin(),
                    hero.hero); // insert hero name at start of vec_data
    std::string csv_data = Utilities::MakeCSV(vec_data);
    ofile << csv_data << "\n";
  }
}

void UpdateHero(Hero &hero) {
  // hero data needs initialised if not already owned
  if (!hero.owned) {
    hero.owned = true;
  }

  size_t input{};

  // ask for and set all data
  std::vector<std::string> grades{"R", "SR", "SSR", "UR", "LR"};
  input = Menu::AskForInput("Please select hero's grade.", grades);
  hero.upgrades[Hero::GRADE] = grades[input - 1];

  std::vector<std::string> levels{"30", "40", "50", "60", "65", "70",
                                  "75", "80", "85", "90", "95", "100"};
  input = Menu::AskForInput("Please select hero's maximum level.", levels);
  hero.upgrades[Hero::LEVEL] = levels[input - 1];

  std::vector<std::string> stars{"0", "1", "2", "3",  "4",  "5", "6",
                                 "7", "8", "9", "10", "11", "12"};
  input = Menu::AskForInput("Please select hero's awakening stars.", stars);
  hero.upgrades[Hero::STARS] = stars[input - 1];

  std::vector<std::string> uniques{"true", "false"};
  input = Menu::AskForInput(
      "Please select whether hero has unlocked their unique ability.", uniques);
  hero.upgrades[Hero::UNIQUE] = uniques[input - 1];

  std::vector<std::string> ultimates{"1", "2", "3", "4", "5", "6"};
  input = Menu::AskForInput("Please select the hero's ultimate move level.",
                            ultimates);
  hero.upgrades[Hero::ULTIMATE] = ultimates[input - 1];
}

void UpdateHeroes(
    const std::vector<std::array<Hero, Heroes::COUNT>::iterator> &list) {
  for (const auto &it : list) {
    auto &hero = *it;
    std::cout << hero.hero << ":" << std::endl;

    bool update{};
    // ask whether to update, with slightly varying wording
    if (hero.owned) {
      // TODO: print upgrades
      update = Menu::YesOrNoInput("Want to update this hero?");
    } else {
      std::cout << "Not owned yet." << std::endl;
      update = Menu::YesOrNoInput("Want to add to owned?");
    }

    if (update) {
      UpdateHero(hero);
    }
  }
}
} // namespace AddOwned
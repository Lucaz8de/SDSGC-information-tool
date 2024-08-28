#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "Conditions.h"
#include "Menu.h"
#include "Hero.h"

// Menu::conditions 

Condition Attribute(const std::vector<std::string> &arguments) {
  std::string attribute = arguments.at(0);
  return [attribute](const Hero &hero) -> bool {
    return hero.get_attribute() == attribute;
  };
}

Condition StartingGrade(const std::vector<std::string> &arguments) {
  std::string grade = arguments.at(0);
  return [grade](const Hero &hero) -> bool {
    return hero.get_starting_grade() == grade;
  };
}

Condition Character(const std::vector<std::string> &arguments) {
  std::string character = arguments.at(0);
  return [character](const Hero &hero) -> bool {
    return hero.get_character() == character;
  };
}

Condition LR(const std::vector<std::string> &arguments) {
  return [](const Hero &hero) -> bool {
    return hero.get_lr();
  };
}

Condition Race(const std::vector<std::string> &arguments) {
  std::string race = arguments.at(0);
  return [race](const Hero &hero) -> bool {
    return hero.get_race() == race;
  };
}

Condition Characteristic(const std::vector<std::string> &arguments) {
  std::string characteristic = arguments.at(0);
  return [characteristic](const Hero &hero) -> bool {
    return hero.get_characteristic() == characteristic;
  };
}

Condition Good(const std::vector<std::string> &arguments) {
  return [](const Hero &hero) -> bool {
    std::vector<int> tiers{ hero.get_pvp_tier1(), hero.get_pve_tier1(), hero.get_pvp_tier2(), hero.get_pve_tier2() };
    return std::any_of(tiers.begin(), tiers.end(), [](int tier) {return tier <= 3; });
  };
}

Condition VeryGood(const std::vector<std::string> &arguments) {
  return [](const Hero &hero) -> bool {
    std::vector<int> tiers{ hero.get_pvp_tier1(), hero.get_pve_tier1(), hero.get_pvp_tier2(), hero.get_pve_tier2() };
    return std::any_of(tiers.begin(), tiers.end(), [](int tier) {return tier <= 2; });
  };
}

Condition Best(const std::vector<std::string> &arguments) {
  return [](const Hero &hero) -> bool {
    std::vector<int> tiers{ hero.get_pvp_tier1(), hero.get_pve_tier1(), hero.get_pvp_tier2(), hero.get_pve_tier2() };
    return std::any_of(tiers.begin(), tiers.end(), [](int tier) {return tier == 1; });
  };
}

Condition AvailableFromDraw(const std::vector<std::string> &arguments) {
  std::string draw = arguments.at(0);
  return [draw](const Hero &hero) -> bool {
    std::vector<std::string> acquisition = hero.get_acquisition();
    return std::find(acquisition.begin(), acquisition.end(), draw) != acquisition.end();
  };
}

Condition Owned(const std::vector<std::string> &arguments) {
  return [](const Hero &hero) -> bool {
    return hero.get_owned();
  };
}

Condition Upgraded(const std::vector<std::string> &arguments) {
  return [](const Hero &hero) -> bool {
    return hero.get_owned() && (hero.upgrades[Hero::GRADE] == "LR" || std::stoi(hero.upgrades[Hero::LEVEL]) > 80);
  };
}

Condition Acquirable(const std::vector<std::string> &arguments) {
  return [](const Hero &hero) -> bool {
    return !(hero.get_acquisition().empty());
  };
}

// Menu::operations

Condition And(const std::vector<Condition> &conditions) {
  return [conditions](const Hero &hero) -> bool {
    return conditions[0](hero) && conditions[1](hero);
  };
}

Condition Or(const std::vector<Condition> &conditions) {
  return [conditions](const Hero &hero) -> bool {
    return conditions[0](hero) || conditions[1](hero);
  };
}

Condition Not(const std::vector<Condition> &conditions) {
  return [conditions](const Hero &hero) -> bool {
    return !(conditions[0](hero));
  };
}
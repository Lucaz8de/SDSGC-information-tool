#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "Conditions.h"
#include "Menu.h"
#include "Hero.h"

Condition And(const std::vector<Condition> &conditions)
{
  return [conditions](const Hero &hero) -> bool {
    return conditions[0](hero) && conditions[1](hero);
  };
}

Condition Or(const std::vector<Condition> &conditions)
{
  return [conditions](const Hero &hero) -> bool {
    return conditions[0](hero) || conditions[1](hero);
  };
}

Condition Not(const std::vector<Condition> &conditions)
{
  return [conditions](const Hero &hero) -> bool {
    return !(conditions[0](hero));
  };
}

Condition AvailableFromDraw(const std::vector<std::string> &arguments)
{
  std::string draw = arguments.at(0);
  return [draw](const Hero &hero) -> bool {
    std::vector<std::string> acquisition = hero.get_acquisition();
    return std::find(acquisition.begin(), acquisition.end(), draw) != acquisition.end();
  };
}

Condition Owned(const std::vector<std::string> &arguments)
{
  return [](const Hero &hero) -> bool {
    return hero.get_owned();
  };
}

Condition Good(const std::vector<std::string> &arguments)
{
  return [](const Hero &hero) -> bool {
    std::vector<int> tiers{ hero.get_pvp_tier1(), hero.get_pve_tier1(), hero.get_pvp_tier2(), hero.get_pve_tier2() };
    return std::any_of(tiers.begin(), tiers.end(), [](int tier) {return tier <= 3; });
  };
}

Condition Upgraded(const std::vector<std::string> &arguments)
{
  return [](const Hero &hero) -> bool {
    return hero.get_owned() && (hero.upgrades[Hero::GRADE] == "LR" || std::stoi(hero.upgrades[Hero::LEVEL]) > 80);
  };
}

Condition Acquirable(const std::vector<std::string> &arguments) {
  return [](const Hero &hero) -> bool {
    return !(hero.get_acquisition().empty());
  };
}

Condition Race(const std::vector<std::string> &arguments) {
  std::string race = arguments.at(0);
  return [race](const Hero &hero) -> bool {
    return hero.get_race() == race;
  };
}
#include <array>
#include <deque>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Hero.h"
#include "Heroes.h"
#include "Utilities.h"

using Utilities::operator<<;

constexpr std::array<size_t, 4> Hero::NUMERICAL_INFORMATION;
constexpr std::array<size_t, 1> Hero::BOOLEAN_INFORMATION;
constexpr std::array<size_t, 3> Hero::NUMERICAL_UPGRADES;
constexpr std::array<size_t, 1> Hero::BOOLEAN_UPGRADES;

Hero::Hero(const std::array<std::string, INFORMATION_COUNT> &data,
           std::deque<std::pair<std::string,
                                std::array<std::string, Hero::UPGRADES_COUNT>>>
               &upgrades,
           const std::unordered_map<std::string, std::vector<std::string>>
               &acquisition)
    : hero{data[0]}, name{data[1]}, attribute{data[2]},
      starting_grade{data[3]}, character{data[4]}, // NOLINT
      lr{data[5] == "true"}, races{Utilities::ParseCSV(data[6])},
      characteristic{data[7]}, // NOLINT
      tiers{std::stoi(data[8]), std::stoi(data[9]), std::stoi(data[10]),
            std::stoi(data[11])}, // NOLINT
      owned{false}, upgrades{}, acquisition{} {
  // heroes are added in the same order, so check if current hero is owned
  auto &next_owned = upgrades.front();
  if (next_owned.first == hero) {
    owned = true;
    this->upgrades = next_owned.second;
    upgrades.pop_front();
  }

  // search for hero in acquisition map in all acquisition methods
  for (const auto &item : acquisition) {
    for (const auto &hero_name : item.second) {
      if (hero == hero_name) {
        this->acquisition.push_back(item.first);
        break;
      }
    }
  }

  // All R and SR characters are available in all draws, except a few exclusive
  // ones
  if ((starting_grade == "R" || starting_grade == "SR") &&
      characteristic != "Collab" && character != "Waillo") {
    this->acquisition.emplace_back("all draws");
  }

  // Every hero gets added to heroes.
  Heroes::list.at(Heroes::count) = *this;
  Heroes::count++;
}

std::ostream &operator<<(std::ostream &os, const Hero &hero) {
  os << "Hero: " << hero.hero << ". Name: " << hero.name
     << ". Attribute: " << hero.attribute
     << ". Starting grade: " << hero.starting_grade
     << ". Character: " << hero.character << ", race(s) "
     << Utilities::MakeCSV(hero.races) << ", of " << hero.characteristic
     << ". Tier " << hero.tiers[0] << " on Amazing's PVP tier list and tier "
     << hero.tiers[1] << " on his PVE tier list. "
     << "Tier " << hero.tiers[2] << " on Nagato's PVP tier list and tier "
     << hero.tiers[3] << " on his PVE tier list. ";
  if (hero.owned) {
    os << "I already own this hero at ultimate level "
       << hero.upgrades[Hero::ULTIMATE] << ".";
  } else {
    if (hero.acquisition.empty()) {
      os << "This hero is not currently acquirable.";
    } else {
      os << "This hero is acquired by " << hero.acquisition << ".";
    }
  }
  return os;
}
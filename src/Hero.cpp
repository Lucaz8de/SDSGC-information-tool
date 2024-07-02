#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Hero.h"
#include "Utilitiesig.h"

/* Note this MUST be defined in a global scope (an interesting exception to private access). 
And for some reason, the type MUST be repeated. */
const std::array<std::string, 7> Hero::lr_names = { "\"Virtual Body Doubles\" Lostvayne Meliodas", "\"Sunshine\" Holy Knight Escanor", "[New Legend] Princess Elizabeth", "[Advent of Flash] Covenant of Light Ludociel", "[The Ten Commandments] Gloxinia of Repose", "[The Ten Commandments] Zeldris of Piety", "[The Ten Commandments] Galland of Truth" };
std::vector<Hero> Hero::heroes = std::vector<Hero>{};

void Hero::MakeHeroes(bool validating) {
	if(validating && !heroes.empty()) {
		throw std::runtime_error("Unexpectedly called MakeHeroes twice.");
	}

	// Read hero acquisition data from acquisition.txt and draws.txt.
	// Make a single hashmap acquisition = { Draw name => [Hero list] }
	std::string DATA_DIR = "../data";
	std::unordered_map<std::string, std::vector<std::string>> acquisition = ReadLists(DATA_DIR + "/acquisition.txt", validating);
	std::unordered_map<std::string, std::vector<std::string>> draws = ReadLists(DATA_DIR + "/draws.txt", validating);
	Merge(acquisition, draws, validating);

	// Read data from owned.txt. Lines are like [Boar Hat] Tavern Master Meliodas, UR, 80, 6, true, 6
	// Make a hashmap upgrades = { [Boar Hat] Tavern Master Meliodas => [UR, 80, 6, true, 6], ... }
	// (Hero::UPGRADEABLE names the elements of the list)
	std::unordered_map<std::string, std::vector<std::string>> upgrades{};
	std::ifstream file{ DATA_DIR + "/owned.txt" };
	while (file.good()) {
		std::string line{};
		std::getline(file, line);
		if (line == "") { // empty lines for grouping, just skip
			continue;
		}
		
		// data = [[Boar Hat] Tavern Master Meliodas, UR, 80, 6, true, 6]
		const std::vector<std::string> data = ParseCS(line, validating);
		if (validating) {
			ValidateList(data, 6, {2, 3, 5}, {4});
		}

		const std::string hero = data[0];
		upgrades[hero] = { data.begin() + 1, data.end() }; 
	}
	file.close();

	// Read data from heroes.txt. Take all the information together and make the Hero objects.
	file.open(DATA_DIR + "/heroes.txt");
	while (file.good()) {
		std::string line{};
		std::getline(file, line);
		if (line == "") { // empty lines for grouping, just skip
			continue;
		}

		// data = [[Boar Hat] Tavern Master Meliodas, Tavern Master Meliodas, Speed, SR, Meliodas, Demon, The Seven Deadly Sins, 5, 5, 5, 5]
		// Hero's named fields name the elements of the list
		const std::vector<std::string> data = ParseCS(line, validating); 
		if (validating) {
			ValidateList(data, 11, {7, 8, 9, 10}, {});
		}

		const Hero hero{ data, upgrades, acquisition };
	}
	file.close();

	if(validating) {
		// validate hero names in owned.txt
		std::vector<std::string> hero_names = HashKeys(upgrades);
		ValidateHeroNames(hero_names, "data/owned.txt");

		// validate hero names in acquisition.txt and draws.txt
		for(const auto& item: acquisition) {
			ValidateHeroNames(item.second, "data/acquisition.txt or data/draws.txt");
		}
	}
}

void Hero::ValidateHeroNames(const std::vector<std::string>& hero_names, std::string check_name) {
	for(std::string hero_name: hero_names) {
		bool hero_found = false;
		for(const Hero& hero: heroes) {
			if(hero.hero == hero_name) {
				hero_found = true;
			}
		}
		if(!hero_found) {
			std::string error_message = "Hero name " + hero_name + " in " + check_name + " was not found in data/heroes.txt.";
			throw std::runtime_error(error_message);
		}
	}
}

Hero::Hero() : hero{}, name{}, attribute{}, starting_grade{}, character{}, race{}, characteristic{},
pvp_tier1{ 0 }, pve_tier1{ 0 }, pvp_tier2{ 0 }, pve_tier2{ 0 }, owned{ false }, acquisition{}, upgrades{}
{}

Hero::Hero(const std::vector<std::string>& data, const std::unordered_map<std::string, std::vector<std::string>>& upgrades, const std::unordered_map<std::string, std::vector<std::string>>& acquisition) :
	hero{ data[0] }, name{ data[1] }, attribute{ data[2] }, starting_grade{ data[3] }, character{ data[4] },
	race{ data[5] }, characteristic{ data[6] }, pvp_tier1{ std::stoi(data[7]) }, pve_tier1{ std::stoi(data[8]) },
	pvp_tier2{ std::stoi(data[9]) }, pve_tier2{ std::stoi(data[10]) }, owned{ false }, acquisition{}, upgrades{}
{
	for (const auto& item : upgrades) { // item is {hero_name: upgrades_list}
		if (hero == item.first) {
			owned = true;
			this->upgrades = item.second;
			break;
		}
	}

	// All R and SR characters are available in all draws, except a few exclusive ones
	if ((starting_grade == "R" || starting_grade == "SR") && characteristic != "Collab" && character != "Waillo") {
		this->acquisition.push_back("all draws");
	}

	for (const auto& item : acquisition) { // item is {acquisition_method: hero_list}
		for (const auto& hero_name : item.second) {
			if (hero == hero_name) {
				this->acquisition.push_back(item.first);
				break;
			}
		}
	}
	std::sort(this->acquisition.begin(), this->acquisition.end());

	// Every hero gets added to heroes.
	heroes.push_back(*this);
}

std::ostream& operator<<(std::ostream& os, const Hero& hero) {
	os << "Hero: " << hero.hero << ". Name: " << hero.name << ". Attribute: " << hero.attribute <<
		". Starting grade: " << hero.starting_grade << ". Character: " << hero.character <<
		", race " << hero.race << ", of " << hero.characteristic << ". Tier " << hero.pvp_tier1 <<
		" on Amazing's PVP tier list and tier " << hero.pve_tier1 << " on his PVE tier list. " <<
		"Tier " << hero.pvp_tier2 << " on Nagato's PVP tier list and tier " << hero.pve_tier2 <<
		" on his PVE tier list. ";
	if (hero.owned) {
		os << "I already own this hero at ultimate level " << hero.upgrades[Hero::ULTIMATE] << ".";
	} 
	else {
		if (hero.acquisition.empty()) {
			os << "This hero is not currently acquirable.";
		}
		else { 
			os << "This hero is acquired by " << hero.acquisition << ".";
		}
	}
	return os;
}
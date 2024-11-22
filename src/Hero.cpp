#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Hero.h"
#include "Menu.h"
#include "Utilities.h"

using Utilities::operator<<;

/* Note this MUST be defined in a global scope (an interesting exception to private access). 
And for some reason, the type MUST be repeated. */
std::vector<Hero> Hero::heroes = std::vector<Hero>{};

void Hero::MakeHeroes() {
	if(!heroes.empty()) {
		throw std::runtime_error("Unexpectedly called MakeHeroes twice.");
	}

	// make heroes list by reading data files
	std::unordered_map<std::string, std::vector<std::string>> acquisition = ReadAcquisitionData();
	std::unordered_map<std::string, std::vector<std::string>> upgrades = ReadOwnedData();
	ReadHeroesData(acquisition, upgrades);

	// upgrades source is owned.csv
	std::vector<std::string> hero_names = Utilities::HashKeys(upgrades);
	ValidateHeroNames(hero_names, "owned.csv");
	// acquisition sources are acquisition.txt and draws.txt
	for(const auto& item: acquisition) {
		std::vector<std::string> hero_names = item.second;
		ValidateHeroNames(hero_names, "acquisition.txt or draws.txt");
	}
}

std::unordered_map<std::string, std::vector<std::string>> Hero::ReadAcquisitionData() {
	std::string DATA_DIR = "../data";
	std::unordered_map<std::string, std::vector<std::string>> acquisition = Utilities::ReadLists(DATA_DIR + "/acquisition.txt");
	std::unordered_map<std::string, std::vector<std::string>> draws = Utilities::ReadLists(DATA_DIR + "/draws.txt");
	Utilities::Merge(acquisition, draws);
	return acquisition;
}

std::unordered_map<std::string, std::vector<std::string>> Hero::ReadOwnedData() {
	// look for owned.csv data file
	std::string DATA_DIR = "../data";
	std::ifstream file{ DATA_DIR + "/owned.csv" };
	if(!file.good()) {
		std::cout << "Data files found, but no information about your owned heroes exists." << std::endl;
		std::cout << "Please select AddOwned from the menu to add owned heroes." << std::endl;
	}

	// process owned.csv data file
	std::unordered_map<std::string, std::vector<std::string>> upgrades{};
	while (file.good()) {
		// read line. skip empty lines for grouping
		std::string line{};
		std::getline(file, line);
		if (Utilities::EmptyCSV(line)) {
			continue;
		}
		
		// read and validate the data
		/* each CSV record is a list like [[Boar Hat] Tavern Master Meliodas, UR, 80, 6, true, 6]
		 * Hero::UPGRADEABLE name the elements of the list i.e. grade, level, awakening stars, unique unlocked, ultimate move level */
		const std::vector<std::string> data = Utilities::ParseCSV(line);
		Utilities::ValidateList(data, 6, {2, 3, 5}, {4});

		// add data to upgrades hashmap
		const std::string hero = data[0];
		upgrades[hero] = { data.begin() + 1, data.end() }; 
	}

	return upgrades;
}

void Hero::ReadHeroesData(std::unordered_map<std::string, std::vector<std::string>> acquisition, std::unordered_map<std::string, std::vector<std::string>> upgrades) {
	std::string DATA_DIR = "../data";
	std::ifstream file{ DATA_DIR + "/heroes.csv" };

	// process heroes.csv data file
	while (file.good()) {
		// read line. skip empty lines for grouping
		std::string line{};
		std::getline(file, line);
		if (Utilities::EmptyCSV(line)) {
			continue;
		}

		// read and validate the data
		/* each CSV record is a list like [[Boar Hat] Tavern Master Meliodas, Tavern Master Meliodas, Speed, SR, Meliodas, false, Demon, The Seven Deadly Sins, 5, 5, 5, 5]
		 * Hero's member attributes name the elements of the list i.e.: hero, name, attribute, starting grade, character, LR, race, characteristic, tiers */
		const std::vector<std::string> data = Utilities::ParseCSV(line); 
		Utilities::ValidateList(data, 12, {8, 9, 10, 11}, {5});

		// call the Hero object constructor.
		const Hero hero{ data, acquisition, upgrades };
	}
}

void Hero::ValidateHeroNames(const std::vector<std::string>& hero_names, std::string source_name) {
	// Checking each hero name in input list...
	for(std::string hero_name: hero_names) {
		bool hero_found = false;
		// Looking in heroes list...
		for(const Hero& hero: heroes) {
			if(hero.hero == hero_name) {
				hero_found = true;
				break;
			}
		}
		if(!hero_found) {
			std::string error_message = "Hero name " + hero_name + " in " + source_name + " was not found in heroes.csv.";
			throw std::runtime_error(error_message);
		}
	}
}

Hero::Hero(const std::vector<std::string>& data, const std::unordered_map<std::string, std::vector<std::string>>& acquisition, const std::unordered_map<std::string, std::vector<std::string>>& upgrades) :
	hero{ data[0] }, name{ data[1] }, attribute{ data[2] }, starting_grade{ data[3] }, character{ data[4] },
	lr{ data[5] == "true" }, races{ Utilities::ParseCSV(data[6]) }, characteristic{ data[7] }, pvp_tier1{ std::stoi(data[8]) }, pve_tier1{ std::stoi(data[9]) },
	pvp_tier2{ std::stoi(data[10]) }, pve_tier2{ std::stoi(data[11]) }, owned{ false }, acquisition{}, upgrades{}
{
	// search for hero in upgrades map
	for (const auto& item : upgrades) {
		if (hero == item.first) {
			owned = true;
			this->upgrades = item.second;
			break;
		}
	}
	// unowned heroes upgrades vector is left empty

	// search for hero in acquisition map in all acquisition methods
	for (const auto& item : acquisition) {
		for (const auto& hero_name : item.second) {
			if (hero == hero_name) {
				this->acquisition.push_back(item.first);
				break;
			}
		}
	}

	// All R and SR characters are available in all draws, except a few exclusive ones
	if ((starting_grade == "R" || starting_grade == "SR") && characteristic != "Collab" && character != "Waillo") {
		this->acquisition.push_back("all draws");
	}

	// Every hero gets added to heroes.
	heroes.push_back(*this);
}

std::ostream& operator<<(std::ostream& os, const Hero& hero) {
	os << "Hero: " << hero.hero << ". Name: " << hero.name << ". Attribute: " << hero.attribute <<
		". Starting grade: " << hero.starting_grade << ". Character: " << hero.character <<
		", race(s) " << Utilities::MakeCSV(hero.races) << ", of " << hero.characteristic << ". Tier " << hero.pvp_tier1 <<
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
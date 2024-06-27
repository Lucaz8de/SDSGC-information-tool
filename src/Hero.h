#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

class Hero {
private:
	// constants
	static const std::array<std::string, 7> lr_names; // This list will contain the names of the LR heroes.
	
	// static variables
	static std::vector<Hero> heroes; // This list will contain all hero objects created.

	// instance variables
	std::string hero; // e.g. [Boar Hat] Tavern Master Meliodas
	std::string name; // e.g. Tavern Master Meliodas
	std::string attribute; // e.g. Speed
	std::string starting_grade; // e.g. SR
	std::string character; // e.g. Meliodas
	std::string race; // e.g. Demon
	std::string characteristic; // e.g. The Seven Deadly Sins
	int pvp_tier1; // Amazing's most recent PVP tierlist
	int pve_tier1; // Amazing's most recent PVE tierlist
	int pvp_tier2; // Nagato's most recent PVP tierlist
	int pve_tier2; // Nagato's most recent PVE tierlist
	bool owned; 
	std::vector<std::string> acquisition;
	std::vector<std::string> upgrades;

public:
	// constants
	enum upgradeable { GRADE, LEVEL, STARS, UNIQUE, ULTIMATE }; // Elements in upgrades list

	// static methods

	/* This function reads all the text files and creates one hero object for all heroes.
	It is the only way hero objects are created and it is called exactly once.
	Note draws and acquisition MUST NOT have common headings. */
	static void MakeHeroes(bool validating);

	/* Utility method for MakeHeroes. Validates hero names.
	 * Throws an exception if any data file uses an unrecognised hero name.
	 */
	static void ValidateHeroNames(const std::vector<std::string>& hero_names, std::string check_name);

	// instance methods

	/* Empty hero object. Not usable. When a vector has to be initialised with a size,
	it has to contain empty hero objects. */
	Hero();

	/* Constructs a hero object and adds it to heroes. */
	Hero(const std::vector<std::string>& data, const std::unordered_map<std::string, std::vector<std::string>>& upgrades, const std::unordered_map<std::string, std::vector<std::string>>& acquisition);

	/* Overload << for heroes. Displays all data except upgrades: hero, name, attribute, starting_grade, 
	character, race, characteristic, tiers, owned and acquisition. */
	friend std::ostream& operator<<(std::ostream& os, const Hero& hero);

	// getters
	static const std::vector<Hero>& get_heroes() {
		return heroes;
	}
	const std::string& get_hero() const {
			return hero;
		}
	const std::string& get_name() const {
		return name;
	}
	const std::string& get_attribute() const {
		return attribute;
	}
	const std::string& get_starting_grade() const {
		return starting_grade;
	}
	const std::string& get_character() const {
		return character;
	}
	const std::string& get_race() const {
		return race;
	}
	const std::string& get_characteristic() const {
		return characteristic;
	}
	int get_pvp_tier1() const {
		return pvp_tier1;
	}
	int get_pve_tier1() const {
		return pve_tier1;
	}
	int get_pvp_tier2() const {
		return pvp_tier2;
	}
	int get_pve_tier2() const {
		return pve_tier2;
	}
	bool get_owned() const {
		return owned;
	}
	const std::vector<std::string>& get_acquisition() const {
		return acquisition;
	}
	const std::vector<std::string>& get_upgrades() const {
		return upgrades;
	}
};
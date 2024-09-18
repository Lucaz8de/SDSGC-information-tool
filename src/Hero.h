/**
 * @file Hero.h
 * @brief Declares Hero class
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @todo write description
 * @brief x
 */
class Hero {
public:
	/** @brief The hero's entire in-game name */
	std::string hero;
	/** @brief The hero's "name" */ 
	std::string name;
	/** @brief The hero's attribute */
	std::string attribute;
	/** @brief The hero's starting grade */
	std::string starting_grade;
	/** @brief The hero's name */
	std::string character;
	/** @brief Whether the hero has an LR grade upgrade */
	bool lr;
	/** @brief The hero's race */
	std::string race;
	/** @brief The hero's characteristic */
	std::string characteristic;
	/** @brief The hero's rank on Amazing's most recent PVP tierlist (a number between 1 and 5) */
	int pvp_tier1; 
	/** @brief The hero's rank on Amazing's most recent PVE tierlist (a number between 1 and 5) */
	int pve_tier1; 
	/** @brief The hero's rank on Nagato's most recent PVP tierlist (a number between 1 and 5) */
	int pvp_tier2; 
	/** @brief The hero's rank on Nagato's most recent PVE tierlist (a number between 1 and 5) */
	int pve_tier2; 
	/** @brief Whether you own the hero */
	bool owned; 
	/** @brief A list of the ways the hero can currently be obtained in-game */
	std::vector<std::string> acquisition;
	/** @brief A list of the hero's upgrades. Empty for unowned heroes. */
	std::vector<std::string> upgrades;

	/** @brief A list the program uses that contains each hero in order
	 * @see MakeHeroes
	 */
	static std::vector<Hero> heroes;

	/** @brief These are the upgrades in the upgrades list */
	enum upgradeable { GRADE, LEVEL, STARS, UNIQUE, ULTIMATE }; 

	/** 
	 * @brief This function processes all the data files. Calls ReadHeroesData, which actually constructs the heroes.
	 * It should be the only way heroes are created and it should be called exactly once.
	 * @throw std::runtime_error if the function is called while heroes is not empty.
	 * @see heroes
	 * @see ReadAcquisitionData
	 * @see ReadOwnedData
	 * @see ReadHeroesData
	 */
	static void MakeHeroes();

	/**
	 * @brief Read hero acquisition data from acquisition.txt and draws.txt.
	 * @return a hashmap { Method name => [Hero list], ... }
	 * @see ReadLists
	 */
	static std::unordered_map<std::string, std::vector<std::string>> ReadAcquisitionData();

	/**
	 * @brief Read owned hero upgrades data from owned.csv.
	 * @return a hashmap { Hero name => [Upgrades list], ... }
	 */
	static std::unordered_map<std::string, std::vector<std::string>> ReadOwnedData();

	/**
	 * @brief Read data from heroes.csv. Construct all the Hero objects, which get added to the heroes list in the constructor.
	 * It should be the only way heroes are created and it should be called exactly once.
	 * @param acquisition The hashmap { Method name => [Hero list], ... }
	 * @param upgrades The hashmap { Hero name => [Upgrades list], ... }
	 * @see Hero
	 */
	static void ReadHeroesData(std::unordered_map<std::string, std::vector<std::string>> acquisition, std::unordered_map<std::string, std::vector<std::string>> upgrades);

	/**
	 * @brief Validates a list of hero names against the program's list, which comes directly from heroes.csv as the source of truth.
	 * @param &hero_names The list of hero names 
	 * @param source_name The name of the source (e.g. data file) of the list of names
	 * @throw std::runtime_error if the list has an unrecognised hero name.
	 */
	static void ValidateHeroNames(const std::vector<std::string>& hero_names, std::string source_name);

	/**
	 * @brief Constructs a hero object, and adds it to heroes.
	 * @param data The list from the line in heroes.csv specific to the hero being constructed
	 * @param acquisition The hashmap { Method name => [Hero list], ... }
	 * @param upgrades The hashmap { Hero name => [Upgrades list], ... } 
	 */
	Hero(const std::vector<std::string>& data, const std::unordered_map<std::string, std::vector<std::string>>& acquisition, const std::unordered_map<std::string, std::vector<std::string>>& upgrades);

	/**
	 * @brief Prints a hero's data, excluding upgrades.
	 * @param &os The outstream that the message will be printed to
	 * @param &hero The hero
	 * @return The outstream that the message prints to
	 */
	friend std::ostream& operator<<(std::ostream& os, const Hero& hero);
};
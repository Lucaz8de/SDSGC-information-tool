/**
 * @file Heroes.h
 * @brief Heroes namespace contains the code to create the list of heroes
 */

#pragma once

#include <array>
#include <cstdlib>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include "Hero.h"

/**
 * @brief Heroes namespace contains the code to create the list of heroes
 */
namespace Heroes {

	/** @brief The number of heroes in the game */
	constexpr size_t COUNT{ 232 };

    /** 
	 * @brief The master list of all heroes; all heroes in the program should be pointers into this list.
	 * @see MakeHeroes
	 */
	std::array<Hero, COUNT> list; // NOLINT

	/** @brief The number of heroes that have been created and added to the list so far */
	size_t count{}; // NOLINT

	/** 
	 * @brief This function processes all the data files. Calls ReadHeroesData, which actually constructs the heroes.
	 * It should be the only way heroes are created and it should be called exactly once.
	 * @throw std::runtime_error if the function is called while heroes is not empty.
	 * @see heroes
	 * @see ReadAcquisitionData
	 * @see ReadOwnedData
	 * @see ReadHeroesData
	 */
	void MakeHeroes();

	/**
	 * @brief Read hero acquisition data from acquisition.txt and draws.txt.
	 * @throw std::runtime_error if the heading names contain duplicates
	 * @return a hashmap { Method name => [Hero list], ... }
	 * @see ReadLists
	 */
	std::unordered_map<std::string, std::vector<std::string>> ReadAcquisitionData();

	/**
	 * @brief Read owned hero upgrades data from owned.csv.
	 * @return a queue { {Hero name, [Upgrades list]}, ... }
	 */
	std::deque<std::pair<std::string, std::array<std::string, Hero::UPGRADES_COUNT>>> ReadOwnedData();

	/**
	 * @brief Read data from heroes.csv. Construct all the Hero objects, which get added to the heroes list in the constructor.
	 * It should be the only way heroes are created and it should be called exactly once.
	 * @param acquisition The hashmap { Method name => [Hero list], ... }
	 * @param upgrades The queue { {Hero name, [Upgrades list]}, ... }
	 * @see Hero
	 */
	void ReadHeroesData(const std::unordered_map<std::string, std::vector<std::string>> &acquisition, std::deque<std::pair<std::string, std::array<std::string, Hero::UPGRADES_COUNT>>> &upgrades);

	/**
	 * @brief Validates a list of hero names against the program's list, which comes directly from heroes.csv as the source of truth.
	 * @param &hero_names The list of hero names 
	 * @param &source_name The name of the source (e.g. data file) of the list of names
	 * @throw std::runtime_error if the list has an unrecognised hero name.
	 */
	void ValidateHeroNames(const std::vector<std::string> &hero_names, const std::string &source_name);
}
#include <algorithm>
#include <fstream>
#include <deque>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "Heroes.h"

#include "Hero.h"
#include "Utilities.h"

namespace Heroes {

    void MakeHeroes() {
        if(!list[0].empty()) {
            throw std::runtime_error("Unexpectedly called MakeHeroes twice.");
        }

        // read data files
        std::unordered_map<std::string, std::vector<std::string>> acquisition = ReadAcquisitionData();
        std::deque<std::pair<std::string, std::array<std::string, Hero::UPGRADES_COUNT>>> upgrades = ReadOwnedData();

        // make copy of owned hero names
        std::vector<std::string> hero_names(upgrades.size());
        for (size_t i{ 0 }; i < upgrades.size(); i++) {
            hero_names[i] = upgrades[i].first;
        }

        ReadHeroesData(acquisition, upgrades);

        // owned sources are owned.csv
        ValidateHeroNames(hero_names, "owned.csv");
        // acquisition sources are acquisition.txt and draws.txt
        for(const auto& item: acquisition) {
            hero_names = item.second;
            ValidateHeroNames(hero_names, "acquisition.txt or draws.txt");
        }
    }

    std::unordered_map<std::string, std::vector<std::string>> ReadAcquisitionData() {
        std::string DATA_DIR = "../data";
        std::unordered_map<std::string, std::vector<std::string>> acquisition = Utilities::ReadLists(DATA_DIR + "/acquisition.txt");
        std::unordered_map<std::string, std::vector<std::string>> draws = Utilities::ReadLists(DATA_DIR + "/draws.txt");
        for(const auto& item : draws) {
            if(acquisition.find(item.first) != acquisition.end()) {
                throw std::runtime_error("The heading names in data/acquisition.txt and data/draws.txt must not contain duplicates.");
            }
            acquisition[item.first] = item.second;
        }
        return acquisition;
    }

    std::deque<std::pair<std::string, std::array<std::string, Hero::UPGRADES_COUNT>>> ReadOwnedData() {
        // look for owned.csv data file
        std::string DATA_DIR = "../data";
        std::ifstream file{ DATA_DIR + "/owned.csv" };
        if(!file.good()) {
            std::cout << "Data files found, but no information about your owned heroes exists." << std::endl;
            std::cout << "Please select AddOwned from the menu to add owned heroes." << std::endl;
        }

        // process owned.csv data file
        std::deque<std::pair<std::string, std::array<std::string, Hero::UPGRADES_COUNT>>> upgrades{};
        while (file.good()) {
            // read line. skip empty lines for grouping
            std::string line{};
            std::getline(file, line);
            if (Utilities::EmptyCSV(line)) {
                continue;
            }
            
            // read and validate the data
            // 
            /* each CSV record is a list like [[Boar Hat] Tavern Master Meliodas, UR, 80, 6, true, 6]
             * int Hero::UPGRADES_COUNT = 5 is the number of upgradeable fields
             * enum Hero::UPGRADEABLE = grade, level, awakening stars, unique unlocked, ultimate move level 
             */
            std::vector<std::string> data = Utilities::ParseCSV(line);
            Utilities::ValidateList(data, Hero::UPGRADES_COUNT+1, Hero::NUMERICAL_UPGRADES, Hero::BOOLEAN_UPGRADES);
  
            // make data array containing upgrades information (excluding data[0] which is hero name)
            std::array<std::string, Hero::UPGRADES_COUNT> data_array{};
            std::copy_n(data.begin()+1, Hero::UPGRADES_COUNT, data_array.begin());

            // add data to upgrades queue
            upgrades.emplace_back(data[0], data_array);
        }

        return upgrades;
    }

    void ReadHeroesData(const std::unordered_map<std::string, std::vector<std::string>> &acquisition, std::deque<std::pair<std::string, std::array<std::string, Hero::UPGRADES_COUNT>>> &upgrades) {
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
            std::vector<std::string> data = Utilities::ParseCSV(line); 
            Utilities::ValidateList(data, Hero::INFORMATION_COUNT, Hero::NUMERICAL_INFORMATION, Hero::BOOLEAN_INFORMATION);

            // make data array containing hero information
            std::array<std::string, Hero::INFORMATION_COUNT> data_array;
            std::copy_n(data.begin(), Hero::INFORMATION_COUNT, data_array.begin());

            // call the Hero object constructor.
            Hero hero{ data_array, upgrades, acquisition };
        }
    }

    void ValidateHeroNames(const std::vector<std::string>& hero_names, const std::string &source_name) {
        // Checking each hero name in input list...
        for(const std::string &hero_name: hero_names) {
            bool hero_found = false;
            // Looking in heroes list...
            for(const Hero& hero: Heroes::list) {
                if(hero.hero == hero_name) {
                    hero_found = true;
                    break;
                }
            }
            if(!hero_found) {
                std::string error_message = "Hero name " + hero_name + " in " + source_name + " was not found in heroes.csv."; // NOLINT
                throw std::runtime_error(error_message);
            }
        }
    }

}
/**
 * @file AddOwned.h 
 * @brief Used by the AddOwned main menu option, to update
 * a list of heroes and then write to the data/owned.csv data file. 
 */

#pragma once
#include "Hero.h"

namespace AddOwned{
    /**
     * @brief (Over)writes file data/owned.csv using current Hero::heroes data.
     */
    void UpdateFile();

    /**
     * @brief Ask for hero's upgrades and update it.
     * @param &hero Reference to the hero object to be updated.
     */
    void UpdateHero(Hero &hero);

    /**
     * @brief Used to AddOwned. Go through a list of heroes in order. Choose yes/no whether to update each one.
     * @param list The list of heroes to update.
     * @todo print upgrades
     */
    void UpdateHeroes(std::vector<Hero> list);
}
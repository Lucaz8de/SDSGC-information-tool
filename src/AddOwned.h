#pragma once
#include "Hero.h"

/**
 * (Over)writes file data/owned.csv using current Hero::heroes data.
 */
void UpdateFile();

/**
 * Ask for hero's upgrades and update it.
 */
void UpdateHero(Hero &hero);

/**
 * Make list of heroes of character and call UpdateHeroes(3, list)
 */
void UpdateHeroes(std::string character);

/**
 * Version with default list argument:
 * Used to AddOwned. Go through heroes list in order. Choose yes/no whether to add each one.
 * Option 1: Only see unowned heroes (add new). 
 * Option 2: Only see owned heroes (upgrade). 
 * Option 3: See all heroes (update). 
 * Other version:
 * For searching by name, called by UpdateHeroes(character) with option = 3.
 */
void UpdateHeroes(int option, std::vector<Hero> list = std::vector<Hero>{});
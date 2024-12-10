/**
 * @file Project.h
 * @brief The main menu functions. They are in the list Menu::menu_functions.
 * The lists need to stay updated - they're what the program uses.
 */

#pragma once

/**
 * This top-level menu option allows you to filter heroes by a selection of
 * conditions. This includes the option to select as many conditions as you
 * want. As well as printing the number of results and the actual results, it
 * keeps track of the conditions selected so that it can be printed. This is the
 * order, left-to-right, that the conditions are evaluated.
 */
void Menu_Filter();

/**
 * This top-level menu option allows you to update your owned heros data in the
 * program and data file, by going through a list of heroes.
 */
void Menu_AddOwned();
#pragma once

/**
 * These functions are in the lists Menu::functions.
 * The lists need to stay updated - they're what the program uses.
 */

/**
 * This top-level menu option allows you to filter heroes by a selection of conditions.
 * This includes the option to select as many conditions as you want.
 * As well as printing the number of results and the actual results,
 * it keeps track of the conditions selected so that it can be printed.
 * This is the order, left-to-right, that the conditions are evaluated.
 */
void Filter();

/**
 * This top-level menu option allows you to update owned.csv in the program.
 */
void AddOwned();
#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Hero.h"

typedef std::function<bool(const Hero &)> Condition;

/**
 * These functions are in the lists Menu::conditions and Menu::operations.
 * The lists need to stay updated - they're what the program uses.
 */

/**
 * All conditions take a list of arguments, and return a Condition.
 * The first few conditions are straightforwardly the hero data fields.
 */

/**
 * Argument list: A single string, the name of an attribute.
 * Return: Condition is true if Hero is that attribute.
 */
Condition Attribute(const std::vector<std::string> &arguments);

/**
 * Argument list: A single string, a starting grade.
 * Return: Condition is true if Hero is that starting grade.
 */
Condition StartingGrade(const std::vector<std::string> &arguments);

/**
 * Argument list: A single string, the name of a character.
 * Return: Condition is true if Hero is that character.
 */
Condition Character(const std::vector<std::string> &arguments);

/**
 * Argument list: Empty.
 * Return: Condition is true if Hero is upgradable to LR.
 */
Condition LR(const std::vector<std::string> &arguments);

/**
 * Argument list: A single string, the name of a race.
 * Return: Condition is true if Hero is that race.
 */
Condition Race(const std::vector<std::string> &arguments);

/**
 * Argument list: A single string, the name of a characteristic.
 * Return: Condition is true if Hero is that characteristic.
 */
Condition Characteristic(const std::vector<std::string> &arguments);

/**
 * Argument list: Empty.
 * Return: Condition is true if Hero is at least tier 3 on any of the tier lists.
 */
Condition Good(const std::vector<std::string> &arguments);

/**
 * Argument list: Empty.
 * Return: Condition is true if Hero is at least tier 2 on any of the tier lists.
 */
Condition VeryGood(const std::vector<std::string> &arguments);

/**
 * Argument list: Empty.
 * Return: Condition is true if Hero is tier 1 on any of the tier lists.
 */
Condition Best(const std::vector<std::string> &arguments);

/**
 * Argument list: Empty.
 * Return: Condition is true if Hero is owned.
 */
Condition Owned(const std::vector<std::string> &arguments);

/**
 * Argument list: Empty.
 * Return: Condition is true if Hero is currently acquirable.
 */
Condition Acquirable(const std::vector<std::string> &arguments);

/**
 * Argument list: A single string, the name of a draw.
 * Return: Condition is true if Hero is available from the draw.
 */
Condition AvailableFromDraw(const std::vector<std::string> &arguments);

/**
 * Argument list: Empty.
 * Return: Condition is true if Hero is owned and above level 80.
 */
Condition Upgraded(const std::vector<std::string> &arguments);

/**
 * All operations take a list of Conditions, and return a Condition.
 */

// Operations
Condition And(const std::vector<Condition> &conditions);
Condition Or(const std::vector<Condition> &conditions);
Condition Not(const std::vector<Condition> &conditions);


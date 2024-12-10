/**
 * @file Filter.h 
 * @brief Used by the Filter main menu option, 
 * to return a list of heroes which can be filtered based on chosen conditions.
 */

#pragma once

#include <functional>
#include <queue>
#include <string>
#include <vector>

#include "Hero.h"
#include "Heroes.h"
#include "Menu.h"

namespace Filter {
    /**
     * @brief An interactive menu to filter heroes, including combining as many conditions
     * as you want. Also prints the conditions selected in the order, left-to-right, 
     * that they are evaluated. (This function is called by the main menu function Filter.) 
     * @return a vector of iterators, pointing to the elements of the original vector. 
     */
    std::vector<std::array<Hero, Heroes::COUNT>::iterator> Filter();

    using Condition = std::function<bool (const Hero &)>;

    /**
     * @brief ConditionFunction lets me easily name and call conditions at runtime.
     */
    struct ConditionFunction { 
        /** @brief a name for the condition */
        std::string name;
        /** @brief the actual callable function */
        std::function<Condition(const std::vector<std::string> &)> function;
    };

    /**
     * @brief Operation lets me easily name and call operations at runtime.
     */
    struct Operation {
        /** @brief a name for the operation */
        std::string name;
        /** @brief the actual callable function */
        std::function<Condition(const std::vector<Condition> &)> function;
        /** @brief the number of conditions the operation is applied to */
        size_t arity;
    };

    /**
     * @brief An interactive menu to choose one condition to filter.
     * @param &filters Reference to a queue of conditions to keep track of them.
     * @return the condition (which is also added to the queue)
     * @see GetArguments
     */
    Condition GetCondition(std::queue<std::string> &filters);

    /**
     * @brief The function that handles deciding which conditions need which arguments,
     * and then calls AskForArguments.
     * @param int_input The user's selection of condition
     * @return the arguments (a vector of strings, possibly empty).
     * @see AskForArguments
     */
    std::vector<std::string> GetArguments(size_t int_input);

    /**
     * @brief An interactive menu to choose the arguments for conditions that need them
     * (in, currently, a limited way).
     * @param name The name of the thing being asked for, e.g. "starting grade"
     * @param options The list of possibilities that name can be, e.g. {"R", "SR", "SSR", "UR"}
     * @param count The number of options to be selected, e.g. 1
     * @return the arguments (a vector of strings, possibly empty).
     */
    std::vector<std::string> AskForArguments(std::string name, std::vector<std::string> options, size_t count);

    /** 
     * @brief An interactive menu to choose an operation (if any) to apply to the condition.
     * Then calls HandleOperation to apply it, including asking for another condition if needed.
     * @param &condition Reference to the initial condition.
     * @param &sentinel_finished Track whether user is finished applying operations
     * (i.e. set to true when they choose to get results).
     * @param &filters Reference to a queue of conditions to keep track of them.
     * @return the resulting condition (e.g. A AND B).
     * @see HandleOperation
     */ 
    Condition GetOperation(Condition &condition, bool &sentinel_finished, std::queue<std::string> &filters);

    /**
     * @brief The function that takes the operation to be applied, and handles it.
     * Calls GetCondition to ask for another condition if needed.
     * @param &condition Reference to the initial condition.
     * @param operation The selected operation to apply to condition.
     * @param &filters Reference to a queue of conditions to keep track of them.
     * @return the resulting condition (e.g. A AND B). (The operation is also added to the queue.)
     * @see GetCondition
     * @throw std::invalid_argument if operation has non-positive 'arity'
     */
    Condition HandleOperation(Condition &condition, Operation operation, std::queue<std::string> &filters);

    /**
     * @name Conditions
     * @brief All conditions take a list of arguments, and return a Condition.
     * The first few conditions are straightforwardly the hero data fields.
     * All the conditions are hardcoded in the list conditions. The list needs
     * to stay updated - it's what the program uses.
     * @{
     */

    /**
     * @param &arguments The vector contains 1 string, the name of an attribute.
     * @return Condition that is true if Hero is that attribute.
     * @throw std::invalid_argument 
     */
    Condition Attribute(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector contains 1 string, the name of a starting grade.
     * @return Condition that is true if Hero is that starting grade.
     * @throw std::invalid_argument 
     */
    Condition StartingGrade(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector contains 1 string, the name of a character.
     * @return Condition that is true if Hero is that character.
     * @throw std::invalid_argument 
     */
    Condition Character(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector is empty.
     * @return Condition that is true if Hero is upgradable to LR.
     * @throw std::invalid_argument 
     */
    Condition LR(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector contains 1 string, the name of a race.
     * @return Condition that is true if Hero is that race.
     * @throw std::invalid_argument 
     */
    Condition Race(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector contains 1 string, the name of a characteristic.
     * @return Condition that is true if Hero is that characteristic.
     * @throw std::invalid_argument 
     */
    Condition Characteristic(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector contains between 1 and 5 numbers, any ranks.
     * @return Condition that is true if Hero is in any of those ranks on any of the tier lists.
     * @throw std::invalid_argument 
     */
    Condition Good(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector is empty.
     * @return Condition that is true if Hero is owned.
     * @throw std::invalid_argument 
     */
    Condition Owned(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector is empty.
     * @return Condition that is true if Hero is currently acquirable.
     * @throw std::invalid_argument 
     */
    Condition Acquirable(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector contains 1 string, the name of a draw.
     * @return Condition that is true if Hero is available from that draw.
     * @throw std::invalid_argument 
     */
    Condition AvailableFromDraw(const std::vector<std::string> &arguments);

    /**
     * @param &arguments The vector is empty.
     * @return Condition that is true if Hero is owned and above level 80.
     * @throw std::invalid_argument 
     */
    Condition Upgraded(const std::vector<std::string> &arguments);

    /**@}*/

    /**
     * @name Operations
     * @brief All operations take a list of Conditions, and return a Condition.
     * All the operations are hardcoded in the list operations. The list needs
     * to stay updated - it's what the program uses.
     * @{
     */

    /**
     * @param &conditions The vector contains 2 conditions
     * @return The result of condition A AND B
     * @throw std::invalid_argument 
     */
    Condition And(const std::vector<Condition> &conditions);

    /**
     * @param &conditions The vector contains 2 conditions
     * @return The result of condition A OR B
     * @throw std::invalid_argument 
     */
    Condition Or(const std::vector<Condition> &conditions);

    /**
     * @param &conditions The vector contains 1 condition
     * @return The result of NOT condition
     * @throw std::invalid_argument 
     */
    Condition Not(const std::vector<Condition> &conditions);

    /**@}*/

    /** @brief Hardcoded list of all conditions */
    const std::vector<ConditionFunction> conditions = {
        {"Attribute", Attribute},
        {"StartingGrade", StartingGrade},
        {"Character", Character},
        {"LR", LR},
        {"Race", Race},
        {"Characteristic", Characteristic},
        {"Good", Good},
        {"Owned", Owned},
        {"Acquirable", Acquirable},
        {"AvailableFromDraw", AvailableFromDraw},
        {"Upgraded", Upgraded},  
    };

    /** @brief Hardcoded list of all operations */
    const std::vector<Operation> operations = {
        {"And", And, 2},
        {"Or", Or, 2},
        {"Not", Not, 1}
    };
}

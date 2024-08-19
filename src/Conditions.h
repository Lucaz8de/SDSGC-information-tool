#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Hero.h"

typedef std::function<bool(const Hero &)> Condition;

// Operations
Condition And(const std::vector<Condition> &conditions);
Condition Or(const std::vector<Condition> &conditions);
Condition Not(const std::vector<Condition> &conditions);

// Conditions

/// <param name="arguments">Contains a single string, the name of a draw.</param>
Condition AvailableFromDraw(const std::vector<std::string> &arguments);

/// <returns>Takes a hero as an input and returns whether it is owned.</returns>
Condition Owned(const std::vector<std::string> &arguments);

/// <returns>Takes a hero as an input and returns whether it is good (any tier &lt;= 3).</returns>
Condition Good(const std::vector<std::string> &arguments);

/// <returns>Takes a hero as an input and returns whether it is upgraded (above level 80).</returns>
Condition Upgraded(const std::vector<std::string> &arguments);

/// <returns>Takes a hero as an input and returns whether it is currently acquirable.</returns>
Condition Acquirable(const std::vector<std::string> &arguments);

/// <param name="arguments">Contains a single string, the name of a certain race.</param>
/// <returns>Takes a hero as an input and returns whether it is of that race.</returns>
Condition Race(const std::vector<std::string> &arguments);
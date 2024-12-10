/**
 * @file Hero.h
 * @brief Declares Hero struct
 */

#pragma once

#include <array>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief A hero is an object of this type; it groups information a hero has.
 */
struct Hero {
  /** @brief The number of pieces of information in heroes.csv file */
  static constexpr size_t INFORMATION_COUNT{12};
  /** @brief The indices that contain numerical data */
  static constexpr std::array<size_t, 4> NUMERICAL_INFORMATION{8, 9, 10, 11};
  /** @brief The indices that contain boolean data */
  static constexpr std::array<size_t, 1> BOOLEAN_INFORMATION{5};
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
  /** @brief The list of the hero's possibly multiple races */
  std::vector<std::string> races;
  /** @brief The hero's characteristic */
  std::string characteristic;
  /** @brief The hero's rank on some different tierlists (a number between 1 and
   * 5) */
  std::array<int, 4> tiers;
  // TODO change the tierlists since Nagato quit lol

  // Section: Owned information (in owned.csv file)
  /** @brief Whether you own the hero */
  bool owned;
  /** @brief The number of upgrades in the upgrades list */
  static constexpr size_t UPGRADES_COUNT{5};
  /** @brief These are the upgrades in the upgrades list */
  enum Upgradeable { GRADE, LEVEL, STARS, UNIQUE, ULTIMATE };
  /** @brief The indices of upgrades that contain numerical data */
  static constexpr std::array<size_t, 3> NUMERICAL_UPGRADES{2, 3, 5};
  /** @brief The indices of upgrades that contain boolean data */
  static constexpr std::array<size_t, 1> BOOLEAN_UPGRADES{4};
  /** @brief A list of the hero's upgrades. Empty for unowned heroes. */
  std::array<std::string, UPGRADES_COUNT> upgrades;

  /** @brief A list of the ways the hero can currently be obtained in-game.
   * Empty for unobtainable heroes. */
  std::vector<std::string> acquisition;

  /**
   * @brief Constructs a hero object, and adds it to heroes.
   * @param data The list from the line in heroes.csv specific to the hero being
   * constructed
   * @param upgrades The queue { {Hero name, [Upgrades list]}, ... }
   * @param acquisition The hashmap { Method name => [Hero list], ... }
   */
  Hero(const std::array<std::string, INFORMATION_COUNT> &data,
       std::deque<std::pair<std::string,
                            std::array<std::string, Hero::UPGRADES_COUNT>>>
           &upgrades,
       const std::unordered_map<std::string, std::vector<std::string>>
           &acquisition);

  /** @brief Default constructor only used to populate empty list */
  Hero() : lr{}, tiers{}, owned{} {}

  /** @brief Check if hero is default constructed */
  bool empty() const { return hero.empty(); }

  /**
   * @brief Prints a hero's data, excluding upgrades.
   * @param &os The outstream that the message will be printed to
   * @param &hero The hero
   * @return The outstream that the message has been printed to
   */
  friend std::ostream &operator<<(std::ostream &os, const Hero &hero);
};
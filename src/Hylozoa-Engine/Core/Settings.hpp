/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Settings related core class [header]
*/

#ifndef SETTINGS_CORE_HPP_
#define SETTINGS_CORE_HPP_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <fstream>
#include <iostream>
#include <istream>

namespace Hylozoa {

/**
 * @struct EngineSettings
 * @brief The structure that holds the engine settings.
 * Instanciate a Settings object and call get() to access the currently loaded
 * settings.
 */
struct EngineSettings {
  public:
    EngineSettings() = default;
    EngineSettings(json &settingsJson);
    // Convert the settings to a json object
    json exportToJson() const;

  public:
    std::string name = "Default Settings"; ///< The name of the settings profile.
    bool verbose = false;     ///< Whether to enable verbose logging or not.
    uint16_t debugLevel = 0; ///< The level of debug information to log (0 = none, higher is more verbose).
}; // struct EngineSettings

/**
 * @class Settings
 * @brief The class that holds the engine settings.
 * The engine settings holds a static settings struct that can be accessed from
 * anywhere by creating an instance of this class.
 */
class Settings {
  private:
    inline static EngineSettings
        _settings{}; ///< The static instance of the settings structure.

  public:
    // This constructor will read from the singletone instance of struct
    // EngineSettings
    Settings() = default;
    // This constructor is used to load settings from a configuration json file
    Settings(const std::string &settingJsonPath);
    // This constructor is used to load settings from a file stream
    Settings(std::istream &jsonStream);
    ~Settings() = default;

    const EngineSettings &get() const { return this->_settings; }

}; // class Settings

} // namespace Hylozoa

std::ostream& operator<<(std::ostream& os, const Hylozoa::EngineSettings& settings);
std::ostream& operator<<(std::ostream& os, const Hylozoa::Settings& settings);

#endif
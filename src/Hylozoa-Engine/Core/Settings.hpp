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
 * @struct _settingsStruct
 * @brief The structure that holds the engine settings.
 * Instanciate a Settings object and call get() to access the currently loaded
 * settings.
 */
struct _settingsStruct {
  public:
    /**
    * @brief Construct a new EngineSettings object with the default values.
    */
    _settingsStruct() = default;
    /**
    * @brief Construct a new EngineSettings object by reading from a JSON object.
    */
    _settingsStruct(json &settingsJson);
    /**
    * @brief Export the current settings to a JSON object.
    */
    json exportToJson() const;

  public:
    std::string name = "Default Settings"; ///< The name of the settings profile.
    bool verbose = false;     ///< Whether to enable verbose logging or not.
    uint16_t debugLevel = 0; ///< The level of debug information to log (0 = none, higher is more verbose).
}; // struct _EngineSettings

/**
 * @class Settings
 * @brief The class that holds the engine settings.
 * The engine settings holds a static settings struct that can be accessed from
 * anywhere by creating an instance of this class.
 */
class Settings {
  private:
    inline static _settingsStruct _settings{}; ///< The static instance of the settings structure.
    // this private constructor is used to prevent instantiation of the Settings class,
    // since it is meant to be used as a singleton
    Settings() = default;

  public:
    /**
    * @brief Retrieve the singletone settings
    * @return Settings&
    */
    static Settings& getInstance() {
      static Settings instance;

      return instance;
    }
    /**
    * @brief Load settings from a configuration json file
    * @param settingJsonPath The path to the JSON file containing the settings
    */
    void load(const std::string &settingJsonPath);
    /**
    * @brief Load settings from a file stream
    * @param jsonStream An input stream containing the JSON data for the settings
    */
    void load(std::istream &jsonStream);
    ~Settings() = default;

    /**
    * @brief Returns the EngineSettings struct that holds the global settings.
    * @return const _settingsStruct&
    */
    const _settingsStruct &getSettings() const { return this->_settings; }

    // Delete copy constructor and assignment operator to prevent copying
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
}; // class Settings

} // namespace Hylozoa

std::ostream& operator<<(std::ostream& os, const Hylozoa::_settingsStruct& settings);
std::ostream& operator<<(std::ostream& os, const Hylozoa::Settings& settings);

#endif
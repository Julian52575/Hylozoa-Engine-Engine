/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Settings related core class [cpp]
*/

#include "Settings.hpp"

inline bool _readFromFile(std::istream &jsonStream, json &outJson) {    
    try {
        jsonStream >> outJson;
    } catch (const json::parse_error &e) {
        std::cout << "Error reading settings json file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

inline bool _readFromFile(const std::string &path, json &outJson) {
    auto stream = std::fstream(path);

    return _readFromFile(stream, outJson);
}


namespace Hylozoa {

    EngineSettings::EngineSettings(json& settingsJson)
    {
        auto _setIfPresent = [&](const std::string& key, auto& member) {
            if (settingsJson.contains(key)) {
                member = settingsJson[key].get<std::decay_t<decltype(member)>>();
            }
        };

        if (settingsJson.is_null()) {
            std::cerr << "Settings JSON is null, using default settings." << std::endl;
            return;
        }
        // Load settings from JSON, with defaults
        _setIfPresent("verbose", this->verbose);
        _setIfPresent("name", this->name);
        _setIfPresent("debugLevel", this->debugLevel);
    }

    json EngineSettings::exportToJson() const
    {
        json j;

        j["name"] = this->name;
        j["verbose"] = this->verbose;
        j["debugLevel"] = this->debugLevel;
        return j;
    }

        
Settings::Settings(std::istream &jsonStream)
{
    json outJson;

    if (! _readFromFile(jsonStream, outJson)) {
        return;
    }
    this->_settings = EngineSettings(outJson);
}

Settings::Settings(const std::string &settingJsonPath) {
    json outJson;

    if (! _readFromFile(settingJsonPath, outJson)) {
        return;
    }
    this->_settings = EngineSettings(outJson);
};

} // namespace Hylozoa

std::ostream& operator<<(std::ostream& os, const Hylozoa::EngineSettings& settings) {
    json j = settings.exportToJson();

    os << j.dump(4);
    return os;
}
std::ostream& operator<<(std::ostream& os, const Hylozoa::Settings& settings) {
    os << settings.get();
    return os;
}
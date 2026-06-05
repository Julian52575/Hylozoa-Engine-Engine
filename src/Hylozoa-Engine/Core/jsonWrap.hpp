/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** small tweak to nlomann basic json [header]
*/

#ifndef JSONWRAP_HPP_
#define JSONWRAP_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

inline std::string toLower(std::string_view str)
{
    std::string result(str);

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char c)
        {
            return std::tolower(c);
        });

    return result;
}


inline void normalizeKeys(json& j)
{
    if (j.is_object())
    {
        json normalized = json::object();

        for (auto& [key, value] : j.items())
        {
            normalized[toLower(key)] = value;
            normalizeKeys(normalized[toLower(key)]);
        }

        j = normalized;
    }
    else if (j.is_array())
    {
        for (auto& item : j)
        {
            normalizeKeys(item);
        }
    }
}


#endif /* !JSONWRAP_HPP_ */
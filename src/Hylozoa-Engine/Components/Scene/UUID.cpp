/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** UUID Components [source file]
*/

#include "UUID.hpp"
#include <iostream>
#include <random>

namespace Hylozoa {

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

UUID::UUID() : m_uuid(s_UniformDistribution(s_Engine)) {
    std::cout << "[UUID] Generated UUID: " << m_uuid << std::endl;
}

} // namespace Hylozoa
/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** LayerManager [source]
*/

#include "LayerManager.hpp"

#include <stdexcept>

namespace Hylozoa {

LayerManager::LayerManager() {
    registerLayer("Default");
    registerLayer("UI");
}

LayerBit LayerManager::registerLayer(const std::string &name) {
    if (m_nameToBit.contains(name)) {
        return m_nameToBit.at(name);
    }

    if (m_nextBitIndex >= 32) {
        throw std::runtime_error("Maximum number of layers reached.");
    }

    LayerBit bit = LayerBit{1u << m_nextBitIndex++};

    m_nameToBit[name] = bit;
    m_bitToName[bit] = name;

    return bit;
}

bool LayerManager::hasLayer(const std::string &name) const {
    return m_nameToBit.find(name) != m_nameToBit.end();
}

LayerBit LayerManager::getLayerBitByName(const std::string &name) const {
    auto it = m_nameToBit.find(name);
    if (it != m_nameToBit.end()) {
        return it->second;
    }

    throw std::runtime_error("Layer not found: " + name);
}

std::string LayerManager::getLayerNameByBit(LayerBit bit) const {
    auto it = m_bitToName.find(bit);

    if (it != m_bitToName.end()) {
        return it->second;
    }

    throw std::runtime_error("Layer bit not found.");
}

std::vector<std::string> LayerManager::maskToNames(LayerMask mask) const {
    std::vector<std::string> result;

    for (const auto &[bit, name] : m_bitToName) {
        if (mask.contains(bit)) {
            result.push_back(name);
        }
    }

    return result;
}

LayerMask
LayerManager::buildMask(const std::vector<std::string> &layers) const {

    if (layers.empty()) {
        return LayerMask::none();
    }

    LayerMask mask = LayerMask::none();

    for (const auto &layerName : layers) {
        if (layerName == "All" || layerName == "all" ||
            layerName == "Everything" || layerName == "everything") {
            return LayerMask::all();
        }

        if (layerName == "None" || layerName == "none" ||
            layerName == "Nothing" || layerName == "nothing") {
            return LayerMask::none();
        }

        auto it = m_nameToBit.find(layerName);
        if (it != m_nameToBit.end()) {
            mask.addLayer(it->second);
        } else {
            throw std::runtime_error("Layer not found: " + layerName);
        }
    }

    return mask;
}

const std::unordered_map<std::string, LayerBit> &LayerManager::layers() const {
    return m_nameToBit;
}

} // namespace Hylozoa
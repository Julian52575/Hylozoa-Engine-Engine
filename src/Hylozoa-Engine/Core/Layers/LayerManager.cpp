/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** LayerManager [source]
*/

#include "LayerManager.hpp"

#include <stdexcept>
#include <iostream>

namespace Hylozoa {

LayerManager::LayerManager() {
}

LayerBit LayerManager::registerLayer(const std::string &name) {
    if (m_nameToBit.contains(name)) {
        return m_nameToBit.at(name);
    }

    if (m_nextBitIndex >= 32) {
        throw std::runtime_error("LayerManager::registerLayer - Maximum number of layers reached.");
    }

    LayerBit bit = LayerBit{1u << m_nextBitIndex++};

    m_nameToBit[name] = bit;
    m_bitToName[bit] = name;

    return bit;
}

void LayerManager::unregisterLayer(const std::string &name) {
    auto it = m_nameToBit.find(name);
    if (it != m_nameToBit.end()) {
        LayerBit bit = it->second;
        m_nameToBit.erase(it);
        m_bitToName.erase(bit);
    } else {
        throw std::runtime_error("LayerManager::unregisterLayer - Layer not found: " + name);
    }
}

bool LayerManager::hasLayer(const std::string &name) const {
    return m_nameToBit.find(name) != m_nameToBit.end();
}

LayerBit LayerManager::getLayerBitByName(const std::string &name) const {
    auto it = m_nameToBit.find(name);
    if (it != m_nameToBit.end()) {
        return it->second;
    }

    std::cerr << "LayerManager::getLayerBitByName - Layer not found: " << name << ". Returning LayerBit{0}.\n";
    return LayerBit{0};
    //throw std::runtime_error("LayerManager::getLayerBitByName - Layer not found: " + name);
}

std::string LayerManager::getLayerNameByBit(LayerBit bit) const {
    auto it = m_bitToName.find(bit);

    if (it != m_bitToName.end()) {
        return it->second;
    }

    std::cerr << "LayerManager::getLayerNameByBit - Layer bit not found. Returning \"None\".\n";
    return "None";
   //throw std::runtime_error("LayerManager::getLayerNameByBit - Layer bit not found.");
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

    bool ErrorOccurred = false;
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
            ErrorOccurred = true;
            std::cerr << "LayerManager::buildMask - Layer not found: " << layerName
                      << ". It will be ignored in the mask.\n";}
    }
    if (ErrorOccurred) {
        std::cerr << "LayerManager::buildMask - Some layers were not found and ignored.\n";
    }
    return mask;
}

const std::unordered_map<std::string, LayerBit> &LayerManager::layers() const {
    return m_nameToBit;
}

} // namespace Hylozoa
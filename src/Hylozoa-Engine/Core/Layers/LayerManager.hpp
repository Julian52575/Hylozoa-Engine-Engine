/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** LayerManager [header]
*/

#ifndef LAYERMANAGER_HPP_
#define LAYERMANAGER_HPP_

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Hylozoa {

using LayerBit = uint32_t;

/**
 * @class LayerMask
 * @brief Represents a mask of layers using bitwise operations.
 *
 * This class allows for the creation and manipulation of layer masks,
 * enabling efficient checks for combinations of layers.
 */
class LayerMask {
  public:
    using Mask = uint32_t;

    LayerMask() = default;
    explicit LayerMask(Mask mask) : m_mask(mask) {}

    /**
     * @brief Checks if the mask contains the specified layer bit.
     */
    bool contains(LayerBit bit) const { return (m_mask & bit) != 0; }

    /**
     * @brief Adds the specified layer bit to the mask.
     */
    void addLayer(LayerBit bit) { m_mask |= bit; }

    /**
     * @brief Removes the specified layer bit from the mask.
     */
    void removeLayer(LayerBit bit) { m_mask &= ~bit; }

    Mask value() const { return m_mask; }

    operator Mask() const { return m_mask; }

    bool operator==(const LayerMask &other) const {
        return m_mask == other.m_mask;
    }

    /**
     * @brief Returns a LayerMask that includes no layers.
     */
    static LayerMask none() { return LayerMask{0}; }
    /**
     * @brief Returns a LayerMask that includes all layers.
     */
    static LayerMask all() { return LayerMask{0xFFFFFFFF}; }

  private:
    Mask m_mask{0};
};

/**
 * @class LayerManager
 * @brief Manages layers and their corresponding bits in the Hylozoa Engine.
 *
 * This class provides functionality to register layers, retrieve layer bits and
 * names, and build layer masks based on layer names. It uses a singleton
 * pattern to ensure a single instance of the LayerManager throughout the
 * engine.
 */
class LayerManager {
  public:
    LayerManager(const LayerManager &) = delete;
    LayerManager &operator=(const LayerManager &) = delete;

    static LayerManager &instance() {
        static LayerManager instance;
        return instance;
    }

    /**
     * @brief Registers a new layer with the given name.
     */
    LayerBit registerLayer(const std::string &name);
    /**
     * @brief Unregisters a layer with the given name.
     */
    void unregisterLayer(const std::string &name);
    /**
     * @brief Checks if a layer with the given name exists.
     */
    bool hasLayer(const std::string &name) const;

    /**
     * @brief Retrieves the LayerBit associated with the given layer name.
     */
    LayerBit getLayerBitByName(const std::string &name) const;
    /**
     * @brief Retrieves the layer name associated with the given LayerBit.
     */
    std::string getLayerNameByBit(LayerBit bit) const;

    /**
     * @brief Builds a LayerMask from a list of layer names.
     */
    LayerMask buildMask(const std::vector<std::string> &layerNames) const;
    /**
     * @brief Converts a LayerMask to a list of layer names.
     */
    std::vector<std::string> maskToNames(LayerMask mask) const;

    /**
     * @brief Returns a map of layer names to their corresponding LayerBits.
     */
    const std::unordered_map<std::string, LayerBit> &layers() const;

  private:
    LayerManager();

    std::unordered_map<std::string, LayerBit> m_nameToBit;
    std::unordered_map<LayerBit, std::string> m_bitToName;
    uint32_t m_nextBitIndex = 0;
};

} // namespace Hylozoa

#endif /* !LAYERMANAGER_HPP_ */
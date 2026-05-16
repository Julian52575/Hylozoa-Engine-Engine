/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** UUID Components [header]
*/

#ifndef UUID_HPP_
#define UUID_HPP_

#include <charconv>
#include <cstdint>
#include <functional>
#include <charconv>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Hylozoa {

/**
 * @class UUID
 * @brief Class to generate and store a unique identifier (UUID).
 *
 * This class encapsulates a unique identifier represented as a 64-bit unsigned
 * integer. The UUID can be used to uniquely identify entities or components
 * within the engine.
 */
class UUID {
  private:
    uint64_t m_uuid;

  public:
    UUID();
    explicit UUID(uint64_t uuid) : m_uuid(uuid) {}
    UUID(const UUID &) = default;
    ~UUID() = default;

    uint64_t value() const noexcept { return m_uuid; }
    operator uint64_t() const { return m_uuid; }

    bool operator==(const UUID &other) const noexcept {
        return m_uuid == other.m_uuid;
    }
};

inline void to_json(json &j, const UUID &uuid) {
    j = std::to_string((uint64_t)uuid);
}

inline void from_json(const json &j, UUID &uuid) {
    if (j.is_string()) {
        std::string s = j.get<std::string>();
        uint64_t value;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);

        if (ec == std::errc()) {
            uuid = UUID(value);
        } else {
            uuid = UUID(0);
        }
    } else if (j.is_number_unsigned()) {
        uuid = UUID(j.get<uint64_t>());
    }
}

namespace Components {
namespace HylozoaInternal {

/**
 * @struct Id
 * @brief Component to store a unique identifier for an entity.
 *
 * This component holds a unique identifier (UUID) for an entity,
 * which can be used for serialization, or other purposes
 * where a consistent ID is required.
 *
 * @var id
 *      The unique identifier for the entity.
 */
struct Id {
    UUID id; // Unique identifier
};

} // namespace HylozoaInternal
} // namespace Components
} // namespace Hylozoa

namespace std {
template <> struct hash<Hylozoa::UUID> {
    std::size_t operator()(const Hylozoa::UUID &uuid) const noexcept {
        return std::hash<uint64_t>{}(uuid.value());
    }
};
} // namespace std

#endif /* !UUID_HPP_ */
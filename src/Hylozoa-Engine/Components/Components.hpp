/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Global Components header
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

// ======== CONTEXT COMPONENTS ========
#include "Context/EngineContext.hpp"
#include "Context/Events.hpp"
#include "Context/Input.hpp"
#include "Context/SceneState.hpp"
#include "Context/Time.hpp"

// ======== CAMERA COMPONENTS ========
#include "Camera/Camera.hpp"

// ======== PHYSICS COMPONENTS ========
#include "Physics/Physics.hpp"

// ======== RENDERING COMPONENTS ========
#include "Rendering/Renderable.hpp"

// ======== SCENE COMPONENTS ========
#include "Scene/Scene.hpp"
#include "Scene/UUID.hpp"

// ======== TRANSFORM COMPONENTS ========
#include "Transform/Transform.hpp"

#include "cereal/archives/json.hpp"

using namespace Hylozoa::Components;

// rapidjson::Writer<rapidjson::StringBuffer>& operator<<(rapidjson::Writer<rapidjson::StringBuffer> &writer, const Hylozoa::Components::Name &name) {
//     writer.StartObject();
//     writer.Key("name");
//     writer.String(name.name.c_str());
//     writer.EndObject();
//     return writer;
// }


namespace cereal
{

template<typename Archive>
void serialize(Archive &archive, Name &name) {
    archive(
        cereal::make_nvp("name", name.name)
    );
}

template<typename Archive>
void serialize(Archive &archive, HylozoaInternal::Id &id) {
    archive(
        cereal::make_nvp("uuid", static_cast<uint64_t>(id.id))
    );
}


template<typename Archive>
void serialize(Archive &archive, LocalTransform &localTransform) {
    archive(
        cereal::make_nvp("position_x", localTransform.position.x),
        cereal::make_nvp("position_y", localTransform.position.y),
        cereal::make_nvp("scale_x", localTransform.scale.x),
        cereal::make_nvp("scale_y", localTransform.scale.y),
        cereal::make_nvp("rotation", localTransform.rotation)
    );
}

template<typename Archive>
void serialize(Archive &archive, Parent &parent) {
    archive(
        cereal::make_nvp("parent", static_cast<std::uint32_t>(parent.entity))
    );
}

} // namespace cereal


#endif

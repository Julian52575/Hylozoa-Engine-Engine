/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform related System [source]
*/

#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Transform.hpp"
#include <unordered_set>

namespace Hylozoa {
namespace Systems {

void ParentChildSystem::updateParentChild(entt::registry &registry) {
    auto child_view = registry.view<Components::HylozoaInternal::Parent>();
    auto parent_view = registry.view<Components::HylozoaInternal::Children>();
    for (auto entity : child_view) {
        auto &parent_comp =
            child_view.get<Components::HylozoaInternal::Parent>(entity);
        if (registry.valid(parent_comp.entity)) {
            auto &child_comp =
                registry.get_or_emplace<Components::HylozoaInternal::Children>(
                    parent_comp.entity);
            child_comp.children.insert(entity);
        }
    }

    for (auto entity : parent_view) {
        auto &children_comp =
            parent_view.get<Components::HylozoaInternal::Children>(entity);

        auto to_remove = std::vector<entt::entity>{};
        to_remove.reserve(children_comp.children.size());
        for (auto child : children_comp.children) {
            if (!registry.valid(child)) {
                to_remove.push_back(child);
            } else if (child_view.contains(child)) {
                auto &parent_comp =
                    child_view.get<Components::HylozoaInternal::Parent>(child);
                if (parent_comp.entity != entity) {
                    to_remove.push_back(child);
                }
            } else {
                to_remove.push_back(child);
            }
        }
        for (auto child : to_remove) {
            children_comp.children.erase(child);
        }
    }
}

// This system compute the worldTransform from the localTransform and the
// parent's worldTransform BUT only if there is no RigidBodyComponent since
// physics will handle that
void UpdateTransformSystem::updateLocalToWorld(entt::registry &registry) {
    auto view = registry.view<Components::LocalTransform>(
        entt::exclude<Components::RigidBodyComponent>);

    std::function<Components::WorldTransform(entt::entity)> compute;

    compute = [&](entt::entity e) -> Components::WorldTransform {
        const auto &local = registry.get<Components::LocalTransform>(e);

        Components::WorldTransform world;

        if (auto parent =
                registry.try_get<Components::HylozoaInternal::Parent>(e)) {
            const auto parentWorld = compute(parent->entity);

            world.scale = parentWorld.scale * local.scale;
            world.rotation = parentWorld.rotation + local.rotation;

            glm::vec2 scaledLocalPos = local.position * parentWorld.scale;
            glm::vec2 rotatedPos =
                rotateVec(scaledLocalPos, parentWorld.rotation);

            world.position = parentWorld.position + rotatedPos;
        } else {
            world.position = local.position;
            world.rotation = local.rotation;
            world.scale = local.scale;
        }

        registry.emplace_or_replace<Components::WorldTransform>(e, world);

        return world;
    };

    for (auto entity : view)
        compute(entity);
}

} // namespace Systems
} // namespace Hylozoa
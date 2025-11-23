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
void parent_child_system(entt::registry &registry) {
  auto child_view = registry.view<Components::Parent>();
  auto parent_view = registry.view<Components::HylozoaInternal::Children>();
  for (auto entity : child_view) {
    auto &parent_comp = child_view.get<Components::Parent>(entity);
    if (registry.valid(parent_comp.entity)) {
      auto &child_comp = registry.get_or_emplace<Components::HylozoaInternal::Children>(
          parent_comp.entity);
      child_comp.childrens.insert(entity);
    }
  }

  for (auto entity : parent_view) {
    auto &children_comp = parent_view.get<Components::HylozoaInternal::Children>(entity);

    auto to_remove = std::vector<entt::entity>{};
    to_remove.reserve(children_comp.childrens.size());
    for (auto child : children_comp.childrens) {
      if (!registry.valid(child)) {
        to_remove.push_back(child);
      } else if (child_view.contains(child)) {
        auto &parent_comp = child_view.get<Components::Parent>(child);
        if (parent_comp.entity != entity) {
          to_remove.push_back(child);
        }
      } else {
        to_remove.push_back(child);
      }
    }
    for (auto child : to_remove) {
      children_comp.childrens.erase(child);
    }
  }
}

// This system compute the worldTransform from the localTransform and the
// parent's worldTransform BUT only if there is no RigidBodyComponent since
// physics will handle that
void local_to_world_system(entt::registry &registry) {
  auto view = registry.view<Components::LocalTransform>();
  std::unordered_set<entt::entity> visited;

  std::function<glm::mat3(entt::entity)> compute = [&](entt::entity e) -> glm::mat3 {
      if (visited.contains(e)) {
          return registry.get<Components::HylozoaInternal::LocalToWorld>(e).matrix;
      }

      glm::mat3 local_matrix(1.0f);
      if (view.contains(e)) {
          const auto& lt = view.get<Components::LocalTransform>(e);
          local_matrix = fromTransform(lt);
      }

      glm::mat3 world_matrix = local_matrix;

      if (auto parent = registry.try_get<Components::Parent>(e)) {
          world_matrix = compute(parent->entity) * world_matrix;
      }

      registry.emplace_or_replace<Components::HylozoaInternal::LocalToWorld>(e, world_matrix);

      if (!registry.all_of<Components::RigidBodyComponent>(e)) {
          registry.emplace_or_replace<Components::WorldTransform>(e, toWorldTransform(world_matrix));
      }

      visited.insert(e);
      return world_matrix;
  };

  for (auto entity : view) {
      compute(entity);
  }
}

} // namespace Hylozoa
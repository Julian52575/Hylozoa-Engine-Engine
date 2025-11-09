/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform related System [source]
*/

#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Transform.hpp"
#include <unordered_set>

namespace Hylozoa {
void parent_child_system(entt::registry &registry) {
  auto child_view = registry.view<Parent>();
  auto parent_view = registry.view<HylozoaInternal::Children>();
  for (auto entity : child_view) {
    auto &parent_comp = child_view.get<Parent>(entity);
    if (registry.valid(parent_comp.entity)) {
      auto &child_comp = registry.get_or_emplace<HylozoaInternal::Children>(
          parent_comp.entity);
      child_comp.childrens.insert(entity);
    }
  }

  for (auto entity : parent_view) {
    auto &children_comp = parent_view.get<HylozoaInternal::Children>(entity);

    auto to_remove = std::vector<entt::entity>{};
    to_remove.reserve(children_comp.childrens.size());
    for (auto child : children_comp.childrens) {
      if (!registry.valid(child)) {
        to_remove.push_back(child);
      } else if (child_view.contains(child)) {
        auto &parent_comp = child_view.get<Parent>(child);
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

void local_to_world_system(entt::registry &registry) {
  auto view = registry.view<LocalTransform>();
  auto visited = std::unordered_set<entt::entity>{};

  // recursive shananigans to traverse the hierarchy
  auto compute_world_matrix = [&](auto &self, entt::entity e) -> matrix3x3 {
    if (visited.find(e) != visited.end()) {
      auto &local_to_world_comp =
          registry.get<HylozoaInternal::LocalToWorld>(e);
      return local_to_world_comp.matrix;
    }
    auto local_matrix = matrix3x3::identity();

    if (view.contains(e)) {
      const auto &local_transform_comp = view.get<LocalTransform>(e);
      local_matrix = matrix3x3::fromTransform(local_transform_comp);
    }

    auto world_matrix = local_matrix;

    auto *parent_comp = registry.try_get<Parent>(e);
    if (parent_comp) {
      auto parent_world_matrix = self(self, parent_comp->entity);
      world_matrix = parent_world_matrix * world_matrix;
    }

    registry.emplace_or_replace<HylozoaInternal::LocalToWorld>(e, world_matrix);

    WorldTransform world_transform = world_matrix.toWorldTransform();
    registry.emplace_or_replace<WorldTransform>(e, world_transform);

    visited.insert(e);

    return world_matrix;
  };

  for (auto entity : view) {
    compute_world_matrix(compute_world_matrix, entity);
  }
}
} // namespace Hylozoa
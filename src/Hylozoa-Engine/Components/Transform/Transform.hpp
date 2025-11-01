/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform Component [header]
*/

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <SDL3/SDL.h>
#include <cmath>
#include <entt/entt.hpp>
#include <numbers>
#include <ostream>
#include <unordered_set>

namespace Hylozoa {

struct WorldTransform;
struct LocalTransform;

class matrix3x3 {
  float m[3][3];

public:
  matrix3x3() {
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
  }

  WorldTransform toWorldTransform();

  static matrix3x3 identity();

  static matrix3x3 fromTransform(const LocalTransform &transform);

  static matrix3x3 scale(float sx, float sy);

  static matrix3x3 rotation(float angle);

  static matrix3x3 translation(float tx, float ty);

  matrix3x3 operator*(const matrix3x3 &other) const {
    matrix3x3 result;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] +
                         m[i][2] * other.m[2][j];
      }
    }
    return result;
  }

  SDL_FPoint operator*(const SDL_FPoint &point) const {
    SDL_FPoint result;
    result.x = m[0][0] * point.x + m[0][1] * point.y + m[0][2];
    result.y = m[1][0] * point.x + m[1][1] * point.y + m[1][2];
    return result;
  }

  SDL_FRect operator*(const SDL_FRect &rect) const {
    SDL_FPoint topLeft = {rect.x, rect.y};
    SDL_FPoint topRight = {rect.x + rect.w, rect.y};
    SDL_FPoint bottomLeft = {rect.x, rect.y + rect.h};
    SDL_FPoint bottomRight = {rect.x + rect.w, rect.y + rect.h};

    topLeft = (*this) * topLeft;
    topRight = (*this) * topRight;
    bottomLeft = (*this) * bottomLeft;
    bottomRight = (*this) * bottomRight;

    float minX = std::min(std::min(topLeft.x, topRight.x),
                          std::min(bottomLeft.x, bottomRight.x));
    float maxX = std::max(std::max(topLeft.x, topRight.x),
                          std::max(bottomLeft.x, bottomRight.x));
    float minY = std::min(std::min(topLeft.y, topRight.y),
                          std::min(bottomLeft.y, bottomRight.y));
    float maxY = std::max(std::max(topLeft.y, topRight.y),
                          std::max(bottomLeft.y, bottomRight.y));

    return {minX, minY, maxX - minX, maxY - minY};
  }

  float *operator[](int index) { return m[index]; }
  const float *operator[](int index) const { return m[index]; }
};

inline float radToDeg(float rad) {
  return rad * 180.0f / std::numbers::pi;
}

inline float degToRad(float deg) {
  return deg * (std::numbers::pi / 180.0f);
}

struct Name {
  std::string name;
};

struct LocalTransform {
  SDL_FPoint position;
  SDL_FPoint scale;
  float rotation; // in RADIANS !!
};

struct WorldTransform {
  SDL_FPoint position;
  SDL_FPoint scale;
  float rotation; // in RADIANS !!
};

struct Parent {
  entt::entity entity{entt::null};
};

namespace HylozoaInternal {
  struct LocalToWorld {
    matrix3x3 matrix;
  };
  
  struct Children {
    std::unordered_set<entt::entity> childrens;
  };
} // namespace HylozoaInternal

} // namespace Hylozoa

#endif /* !TRANSFORM_HPP_ */
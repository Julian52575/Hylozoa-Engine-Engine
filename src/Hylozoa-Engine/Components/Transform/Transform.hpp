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

/*
 * @class matrix3x3
 * @brief A class representing a 3x3 matrix for 2D transformations.
 *
 * This class provides methods for creating and manipulating 3x3 matrices,
 * which can be used for various 2D transformations such as translation,
 * rotation, and scaling.
 */
class matrix3x3 {
  float m[3][3];

public:
  /*
   * @brief Constructs an identity matrix.
   */
  matrix3x3() {
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
  }

  /*
   * @brief Converts the matrix to a WorldTransform.
   */
  WorldTransform toWorldTransform();

  /*
   * @brief Creates and returns an identity matrix.
   */
  static matrix3x3 identity();

  /*
   * @brief Creates a matrix from a LocalTransform.
   */
  static matrix3x3 fromTransform(const LocalTransform &transform);

  /*
   * @brief Creates a scaling matrix.
   */
  static matrix3x3 scale(float sx, float sy);

  /*
   * @brief Creates a rotation matrix.
   */
  static matrix3x3 rotation(float angle);

  /*
   * @brief Creates a translation matrix.
   */
  static matrix3x3 translation(float tx, float ty);

  /*
   * @brief Multiplies this matrix by another matrix.
   */
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

  /*
   * @brief Transforms a point using this matrix.
   */
  SDL_FPoint operator*(const SDL_FPoint &point) const {
    SDL_FPoint result;
    result.x = m[0][0] * point.x + m[0][1] * point.y + m[0][2];
    result.y = m[1][0] * point.x + m[1][1] * point.y + m[1][2];
    return result;
  }

  /*
   * @brief Transforms a rectangle using this matrix.
   */
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

  /*
   * @brief Accesses a row of the matrix.
   */
  float *operator[](int index) { return m[index]; }
  /*
   * @brief Accesses a row of the matrix (const version).
   */
  const float *operator[](int index) const { return m[index]; }
};

/*
 * @brief Converts radians to degrees.
 */
inline float radToDeg(float rad) { return rad * 180.0f / std::numbers::pi; }

/*
 * @brief Converts degrees to radians.
 */
inline float degToRad(float deg) { return deg * (std::numbers::pi / 180.0f); }

/*
 * @struct Name
 * @brief Component to store the name of an entity.
 * This component holds a string representing the name assigned to an entity.
 *
 * @var string name
 */
struct Name {
  std::string name;
};

/*
 * @struct LocalTransform
 * @brief Component to store local transformation data.
 * This component holds the position, scale, and rotation of an entity relative
 * to its parent.
 *
 * @var SDL_FPoint position
 *      The local position of the entity.
 * @var SDL_FPoint scale
 *      The local scale of the entity.
 * @var float rotation
 *      The local rotation of the entity in radians.
 */
struct LocalTransform {
  SDL_FPoint position;
  SDL_FPoint scale;
  float rotation; // in RADIANS !!
};

/*
 * @struct WorldTransform
 * @brief Component to store world transformation data.
 * This component holds the position, scale, and rotation of an entity in world
 * space.
 *
 * @var SDL_FPoint position
 *      The world position of the entity.
 * @var SDL_FPoint scale
 *      The world scale of the entity.
 * @var float rotation
 *      The world rotation of the entity in radians.
 */
struct WorldTransform {
  SDL_FPoint position;
  SDL_FPoint scale;
  float rotation; // in RADIANS !!
};

/*
 * @struct Parent
 * @brief Component to store the parent entity reference.
 * This component holds a reference to the parent entity of the current entity.
 *
 * @var entt::entity entity
 *      The parent entity reference.
 */
struct Parent {
  entt::entity entity{entt::null};
};

/*
 * @namespace HylozoaInternal
 * @brief Internal components for managing entity relationships and
 * transformations.
 */
namespace HylozoaInternal {
/*
 * @struct LocalToWorld
 * @brief Component to store the local-to-world transformation matrix.
 */
struct LocalToWorld {
  matrix3x3 matrix;
};

/*
 * @struct Children
 * @brief Component to store the set of child entities.
 */
struct Children {
  std::unordered_set<entt::entity> childrens;
};
} // namespace HylozoaInternal

} // namespace Hylozoa

#endif /* !TRANSFORM_HPP_ */
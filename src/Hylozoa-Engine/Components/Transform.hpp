/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform Component [header]
*/

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <SDL3/SDL.h>
#include <numbers>
#include <cmath>
#include <ostream>

namespace Hylozoa {

struct Transform2D {
    float x;
    float y;
};

struct LocalTransform {
    SDL_FPoint position;
    SDL_FPoint scale;
    float rotation; // in degrees
};


struct GlobalTransform {
    SDL_FPoint position;
    SDL_FPoint scale;
    float rotation; // in degrees
};



// float deg2Rad(float degrees) {
//     return degrees * (std::numbers::pi / 180.0f);
// }

class matrix3x3 {
    float m[3][3];
public:
    matrix3x3() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = 0;
    }

    static matrix3x3 identity() {
        matrix3x3 newMatrix;
        newMatrix.m[0][0] = 1; newMatrix.m[0][1] = 0; newMatrix.m[0][2] = 0;
        newMatrix.m[1][0] = 0; newMatrix.m[1][1] = 1; newMatrix.m[1][2] = 0;
        newMatrix.m[2][0] = 0; newMatrix.m[2][1] = 0; newMatrix.m[2][2] = 1;
        return newMatrix;
    }

    GlobalTransform toGlobalTransform() const {
        GlobalTransform gt;
        gt.position = {m[0][2], m[1][2]};
        gt.scale = {std::sqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0]),
                    std::sqrt(m[0][1] * m[0][1] + m[1][1] * m[1][1])};
        gt.rotation = std::atan2(m[1][0], m[0][0]) * (180.0f / std::numbers::pi);
        return gt;
    }

    static matrix3x3 fromTransform(const LocalTransform& transform) {
        matrix3x3 newMatrix;
        float rad = transform.rotation * (std::numbers::pi / 180.0f);
        float cosA = std::cos(rad);
        float sinA = std::sin(rad);

        newMatrix.m[0][0] = transform.scale.x * cosA;
        newMatrix.m[0][1] = -transform.scale.y * sinA;
        newMatrix.m[0][2] = transform.position.x;
        newMatrix.m[1][0] = transform.scale.x * sinA;
        newMatrix.m[1][1] = transform.scale.y * cosA;
        newMatrix.m[1][2] = transform.position.y;
        newMatrix.m[2][0] = 0;
        newMatrix.m[2][1] = 0;
        newMatrix.m[2][2] = 1;

        return newMatrix;
    }

    static matrix3x3 fromTransform(const GlobalTransform& transform) {
        matrix3x3 newMatrix;
        float rad = transform.rotation * (std::numbers::pi / 180.0f);
        float cosA = std::cos(rad);
        float sinA = std::sin(rad);

        newMatrix.m[0][0] = transform.scale.x * cosA;
        newMatrix.m[0][1] = -transform.scale.y * sinA;
        newMatrix.m[0][2] = transform.position.x;
        newMatrix.m[1][0] = transform.scale.x * sinA;
        newMatrix.m[1][1] = transform.scale.y * cosA;
        newMatrix.m[1][2] = transform.position.y;
        newMatrix.m[2][0] = 0;
        newMatrix.m[2][1] = 0;
        newMatrix.m[2][2] = 1;

        return newMatrix;
    }

    static matrix3x3 scale(float sx, float sy) {
        matrix3x3 scaleMatrix;
        scaleMatrix.m[0][0] = sx; scaleMatrix.m[0][1] = 0;  scaleMatrix.m[0][2] = 0;
        scaleMatrix.m[1][0] = 0;  scaleMatrix.m[1][1] = sy; scaleMatrix.m[1][2] = 0;
        scaleMatrix.m[2][0] = 0;  scaleMatrix.m[2][1] = 0;  scaleMatrix.m[2][2] = 1;
        return scaleMatrix;
    }

    static matrix3x3 rotation(float angle) {
        float rad = angle * (std::numbers::pi / 180.0f);
        float cosA = std::cos(rad);
        float sinA = std::sin(rad);

        matrix3x3 rotationMatrix;
        rotationMatrix.m[0][0] = cosA;  rotationMatrix.m[0][1] = -sinA; rotationMatrix.m[0][2] = 0;
        rotationMatrix.m[1][0] = sinA;  rotationMatrix.m[1][1] = cosA;  rotationMatrix.m[1][2] = 0;
        rotationMatrix.m[2][0] = 0;     rotationMatrix.m[2][1] = 0;     rotationMatrix.m[2][2] = 1;

        return rotationMatrix;
    }

    static matrix3x3 translation(float tx, float ty) {
        matrix3x3 translationMatrix;
        translationMatrix.m[0][0] = 1; translationMatrix.m[0][1] = 0; translationMatrix.m[0][2] = tx;
        translationMatrix.m[1][0] = 0; translationMatrix.m[1][1] = 1; translationMatrix.m[1][2] = ty;
        translationMatrix.m[2][0] = 0; translationMatrix.m[2][1] = 0; translationMatrix.m[2][2] = 1;
        return translationMatrix;
    }

    matrix3x3 operator*(const matrix3x3& other) const {
        matrix3x3 result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result.m[i][j] = m[i][0] * other.m[0][j] +
                                 m[i][1] * other.m[1][j] +
                                 m[i][2] * other.m[2][j];
            }
        }
        return result;
    }

    SDL_FPoint operator*(const SDL_FPoint& point) const {
        SDL_FPoint result;
        result.x = m[0][0] * point.x + m[0][1] * point.y + m[0][2];
        result.y = m[1][0] * point.x + m[1][1] * point.y + m[1][2];
        return result;
    }

    SDL_FRect operator*(const SDL_FRect& rect) const {
        SDL_FPoint topLeft = {rect.x, rect.y};
        SDL_FPoint topRight = {rect.x + rect.w, rect.y};
        SDL_FPoint bottomLeft = {rect.x, rect.y + rect.h};
        SDL_FPoint bottomRight = {rect.x + rect.w, rect.y + rect.h};

        topLeft = (*this) * topLeft;
        topRight = (*this) * topRight;
        bottomLeft = (*this) * bottomLeft;
        bottomRight = (*this) * bottomRight;

        float minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
        float maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
        float minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
        float maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));

        return {minX, minY, maxX - minX, maxY - minY};
    }

    float* operator[](int index) {
        return m[index];
    }
    const float* operator[](int index) const {
        return m[index];
    }
};

} // namespace Hylozoa


#endif /* !TRANSFORM_HPP_ */
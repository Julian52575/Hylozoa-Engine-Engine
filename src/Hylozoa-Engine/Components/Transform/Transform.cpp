/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform Component [source]
*/

#include "Transform.hpp"

namespace Hylozoa {

matrix3x3 matrix3x3::identity() {
    matrix3x3 newMatrix;
    return newMatrix;
}

matrix3x3 matrix3x3::fromTransform(const LocalTransform &transform) {
    matrix3x3 newMatrix;

    float rad = transform.rotation; // already radians
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

matrix3x3 matrix3x3::scale(float sx, float sy) {
    matrix3x3 newMatrix;
    newMatrix.m[0][0] = sx;
    newMatrix.m[1][1] = sy;

    return newMatrix;
}

matrix3x3 matrix3x3::rotation(float rad) {
    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    matrix3x3 newMatrix;
    newMatrix.m[0][0] = cosA;
    newMatrix.m[0][1] = -sinA;
    newMatrix.m[1][0] = sinA;
    newMatrix.m[1][1] = cosA;

    return newMatrix;
}

matrix3x3 matrix3x3::translation(float tx, float ty) {
    matrix3x3 newMatrix;
    newMatrix.m[0][2] = tx;
    newMatrix.m[1][2] = ty;

    return newMatrix;
}

WorldTransform matrix3x3::toWorldTransform() {
    WorldTransform wt;

    wt.position.x = m[0][2];
    wt.position.y = m[1][2];

    float scaleX = std::sqrt(m[0][0]*m[0][0] + m[1][0]*m[1][0]);
    float scaleY = std::sqrt(m[0][1]*m[0][1] + m[1][1]*m[1][1]);
    wt.scale = {scaleX, scaleY};

    float rot = std::atan2(m[1][0] / scaleX, m[0][0] / scaleX);
    wt.rotation = rot;

    return wt;
}

} // namespace Hylozoa
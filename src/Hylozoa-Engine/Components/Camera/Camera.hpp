/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Camera Components [header]
*/

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "Hylozoa-Engine/Core/Layers/LayerManager.hpp"
#include <glm/glm.hpp>

namespace Hylozoa {
namespace Components {
/**
 * @struct Camera
 * @brief Represents a camera component for rendering.
 *
 * This component defines the properties of a camera used for rendering the
 * scene.
 *
 * @var viewportSize
 *      The size of the camera's viewport in pixels.
 * @var offset
 *     The offset of the camera from its target position.
 * @var zoom
 *      The zoom level of the camera.
 * @var order
 *     The rendering order of the camera.
 * @var cullingMask
 *     The layers that the camera will render.
 * @var isUI
 *     Indicates if the camera is used for UI rendering.
 */
struct Camera {
    glm::vec2 viewportSize{1920.0f, 1080.0f}; // Default viewport size
    glm::vec2 offset{0.0f, 0.0f};             // Camera offset
    float zoom{1.0f};                         // Default zoom level
    int order{0};                             // Camera rendering order
    LayerMask cullingMask = LayerMask::all(); // Layers to render
    bool isUI{false};                         // Is this a UI camera
};
} // namespace Components
} // namespace Hylozoa

#endif /* !CAMERA_HPP_ */
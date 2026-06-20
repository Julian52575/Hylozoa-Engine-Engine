/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** API macros for cross-platform symbol export
*/

#ifndef ENGINE_API_MACROS_HPP
#define ENGINE_API_MACROS_HPP

#ifdef _WIN32
#define API_EXPORT extern "C" __declspec(dllexport)
#else
#define API_EXPORT extern "C" __attribute__((visibility("default")))
#endif

namespace Hylozoa {
class Engine;
}

#define ENGINE_PTR Hylozoa::Engine *

#endif // ENGINE_API_MACROS_HPP
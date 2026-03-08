/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Listener Components [header]
*/

#ifndef LISTENER_HPP_
#define LISTENER_HPP_

namespace Hylozoa {
namespace Components {

    /*
    * @struct MainListener
    * @brief Tag Component to mark the main audio listener in the scene.
    *
    * This component is used to identify the primary audio listener entity in the scene.
    * It does not contain any data, but its presence indicates that the entity should be
    * treated as the main listener to actually hear the sounds in the scene.
    * 
    * \warning
    * Only one entity should have this component at a time.
    * 
    * \type
    * Tag Component
    */
    struct MainListener {};

    /*
    * @struct NoiseListener
    * @brief Component to define if an entity can hear noises
    * 
    * This component is used to mark entities that can hear noises within a certain radius.
    * When a noise event occurs, the engine will check for entities with this component and determine if they are within the `hearingRange` of the noise source to decide if they should receive the noise event.
    */
    struct NoiseListener {
        float hearingRange{100.0f}; // The range within which the listener can hear noises, in world units
    };

} // namespace Components

} // namespace Hylozoa


#endif /* !LISTENER_HPP_ */
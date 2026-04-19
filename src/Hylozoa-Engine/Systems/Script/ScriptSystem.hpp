/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Script related System [header]
*/

#ifndef SCRIPT_SYSTEM_HPP_
#define SCRIPT_SYSTEM_HPP_

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"

namespace Hylozoa::Systems {

class ScriptSystem : public System {
    public:
        ScriptSystem(entt::registry &registry) : System(registry) {};
        const std::string &getName() const override { return this->_name; };

        void onStart() override;
        void onUpdate(float dt) override;
        void onSceneLoaded(const uint64_t sceneId) override;
        void onSceneUnloaded(const uint64_t sceneId) override;
        void onEnd() override;
    private:
        std::string _name = "ScriptSystem";

};

}


#endif /* !SCRIPT_SYSTEM_HPP_ */
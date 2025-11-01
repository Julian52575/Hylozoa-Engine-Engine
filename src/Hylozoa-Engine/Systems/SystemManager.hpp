#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include "Hylozoa-Engine/Systems/Systems.hpp"
#include <memory>
#include <unordered_map>
#include <iostream>
#include <typeindex>

namespace Hylozoa {
    class SystemManager {
        private:
            std::unordered_map<std::type_index, std::unique_ptr<System>> _systems;
            entt::registry& _registry;
        public:
            SystemManager(entt::registry& registry) : _registry(registry) {}

            template<typename T, typename... Args>
            T* registerSystem(Args&&... args) {
                auto type = std::type_index(typeid(T));
                auto system = std::make_unique<T>(std::forward<Args>(args)...);
                T* ptr = system.get();

                system->setRegistry(&_registry);

                this->_systems[type] = std::move(system);
                return ptr;
            }

            template<typename T>
            T* getSystem() {
                auto it = this->_systems.find(std::type_index(typeid(T)));
                if (it != this->_systems.end()) {
                    return static_cast<T*>(it->second.get());
                }
                return nullptr;
            }

            void startAll() {
                for (auto& [type, system] : this->_systems) {
                    if (!system->isActive()) {
                        system->onStart();
                        system->setActive(true);
                    }
                }
            }

            void endAll() {
                for (auto& [type, system] : this->_systems) {
                    if (system->isActive()) {
                        system->onEnd();
                        system->setActive(false);
                    }
                }
            }

            void updateAll(float deltaTime) {
                for (auto& [type, system] : this->_systems) {
                    if (system->isActive()) {
                        system->onUpdate(deltaTime);
                    }
                }
            }

            void setActive(System* system, bool active) {
                if (system) {
                    system->setActive(active);
                }
            }
    };
};



#endif // SYSTEM_MANAGER_HPP
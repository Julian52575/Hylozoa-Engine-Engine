#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <memory>
#include <unordered_map>
#include <iostream>
#include <typeindex>

namespace Hylozoa {
    class SystemManager {
        private:
            std::unordered_map<std::type_index, std::unique_ptr<System>> _systems;
            std::unordered_map<std::type_index, std::unique_ptr<System>> _fixedSystems;
            std::vector<System*> _systemOrder;
            std::vector<System*> _fixedSystemOrder;
            entt::registry& _registry;
        public:
            SystemManager(entt::registry& registry) : _registry(registry) {}

            template<typename T, typename... Args>
            T* registerSystem(int priority, Args&&... args) {
                auto type = std::type_index(typeid(T));
                auto system = std::make_unique<T>(std::forward<Args>(args)...);
                T* ptr = system.get();

                system->setRegistry(&_registry);
                system->setPriority(priority);

                this->_systems[type] = std::move(system);
                return ptr;
            }

            template<typename T, typename... Args>
            T* registerFixedSystem(int priority, Args&&... args) {
                auto type = std::type_index(typeid(T));
                auto system = std::make_unique<T>(std::forward<Args>(args)...);
                T* ptr = system.get();

                system->setRegistry(&_registry);
                system->setPriority(priority);

                this->_fixedSystems[type] = std::move(system);
                return ptr;
            }

            template<typename T>
            T* getSystem() {
                auto it = this->_systems.find(std::type_index(typeid(T)));
                if (it != this->_systems.end()) {
                    return static_cast<T*>(it->second.get());
                }
                if (auto fit = this->_fixedSystems.find(std::type_index(typeid(T))); fit != this->_fixedSystems.end()) {
                    return static_cast<T*>(fit->second.get());
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
                for (auto& [type, system] : this->_fixedSystems) {
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
                for (auto& [type, system] : this->_fixedSystems) {
                    if (system->isActive()) {
                        system->onEnd();
                        system->setActive(false);
                    }
                }
            }

            void updateAll(float deltaTime) {
                for (auto& system : this->_systemOrder) {
                    if (system->isActive()) {
                        system->onUpdate(deltaTime);
                    }
                }
                for (auto& system : this->_fixedSystemOrder) {
                    if (system->isActive()) {
                        system->onUpdate(deltaTime);
                    }
                }
            }

            void update(float deltaTime) {
                for (auto& system : this->_systemOrder) {
                    if (system->isActive()) {
                        system->onUpdate(deltaTime);
                    }
                }
            }

            void updateFixed(float deltaTime) {
                for (auto& system : this->_fixedSystemOrder) {
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

            void orderAllSystems() {
                _systemOrder.clear();
                _fixedSystemOrder.clear();
                for (auto& [_, sys] : _systems)
                    _systemOrder.push_back(sys.get());

                std::sort(_systemOrder.begin(), _systemOrder.end(),
                        [](System* a, System* b) {
                            std::cout << "Comparing " << a->getName() << " (priority " << a->getPriority() << ") with " << b->getName() << " (priority " << b->getPriority() << ")\n";
                            return a->getPriority() < b->getPriority();
                        });
                for (auto& [_, sys] : _fixedSystems)
                    _fixedSystemOrder.push_back(sys.get());

                std::sort(_fixedSystemOrder.begin(), _fixedSystemOrder.end(),
                        [](System* a, System* b) {
                            return a->getPriority() < b->getPriority();
                        });
            }
    };
};



#endif // SYSTEM_MANAGER_HPP
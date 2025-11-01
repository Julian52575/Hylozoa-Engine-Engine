#ifndef SYSTEMS_HPP_
#define SYSTEMS_HPP_


namespace Hylozoa {
    class System {
        public:
            virtual ~System() = default;

            virtual void onStart() = 0;
            virtual void onUpdate(float deltaTime) = 0;
            virtual void onEnd() = 0;

            virtual const std::string& getName() const = 0;

            bool isActive() const { return _isActive; }
            void setActive(bool active) { _isActive = active; }

            void setRegistry(entt::registry* reg) { _registry = reg; }
        
        protected:
            bool _isActive = false;
            entt::registry* _registry = nullptr;
    };
}

#endif // SYSTEMS_HPP_

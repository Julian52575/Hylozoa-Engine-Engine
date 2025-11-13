#ifndef SYSTEMS_HPP_
#define SYSTEMS_HPP_

#include <entt/entt.hpp>

namespace Hylozoa {
class System {
public:
  virtual ~System() = default;

  virtual void onStart() = 0;
  virtual void onUpdate(float deltaTime) = 0;
  virtual void onEnd() = 0;

  virtual const std::string &getName() const = 0;

  bool isActive() const { return _isActive; }
  System &setActive(bool active) {
    _isActive = active;
    return *this;
  }

  System &setPriority(int priority) {
    _priority = priority;
    return *this;
  }

  System &setRegistry(entt::registry *reg) {
    _registry = reg;
    return *this;
  }

  const int getPriority() const { return _priority; }

protected:
  bool _isActive = false;
  entt::registry *_registry = nullptr;
  int _priority = 0; // Used for systems execution order the lower the number
                     // the higher the priority
};
} // namespace Hylozoa

#endif // SYSTEMS_HPP_

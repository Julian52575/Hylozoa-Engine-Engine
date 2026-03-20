#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <entt/entt.hpp>

namespace Hylozoa::Systems {
class Movement : public System {
  public:
    Movement(entt::registry &registry) : System(registry) {};
    ~Movement() = default;
    void onStart() override;
    void onUpdate(float deltaTime) override;
    void onEnd() override;
    const std::string &getName() const override;

  protected:
    int _priority = 3;
    std::string _name = "MovementSystem";
};
} // namespace Hylozoa::Systems
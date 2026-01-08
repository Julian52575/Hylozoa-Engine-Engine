#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"

namespace Hylozoa::Systems {
class Movement : public System {
public:
  Movement();
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
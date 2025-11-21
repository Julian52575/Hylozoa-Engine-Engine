#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <SDL3/SDL.h>

namespace Hylozoa::Systems {
class Input : public System {
public:
  Input();
  ~Input() = default;
  void onStart() override;
  void onUpdate(float deltaTime) override;
  void onEnd() override;
  const std::string &getName() const override;

protected:
  int _priority = 2;
  std::string _name = "InputSystem";

private:
  SDL_Event event;
};
} // namespace Hylozoa::Systems
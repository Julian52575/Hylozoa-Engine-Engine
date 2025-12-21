#include "EngineFixture.hpp"
#include "Hylozoa-Engine/Components/Context/Input.hpp"

class InputFixture : public HeadlessEngineFixture
{
protected:
    Hylozoa::Components::HylozoaInternal::InputState& inputState;
private:
public:
    InputFixture() : inputState(engine.getRegistry().ctx().get<Hylozoa::Components::HylozoaInternal::InputState>()) {}
};


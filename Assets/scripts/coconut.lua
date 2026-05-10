--function onStart(entity)
--  print("Crab onStart")
--end

local lastYPosition = 1234
local groundBuffer = 0
local groundDispawnDelay = 2.0
local fallSpeed = 10.0

function onUpdate(entity, dt)
    -- Remove if grounded for X seconds
    if groundBuffer >= groundDispawnDelay then
      destroy_entity(entity)
      return
    end
    -- No update on Y position so we consider grounded
    if lastYPosition == get_transform(entity).position.y then
      groundBuffer = groundBuffer + dt
    else -- Reset if movement
      groundBuffer = 0
    end
    lastYPosition = get_transform(entity).position.y
    ---- TMP gravity simulation
    ---- Uncomment to debug: Position changes but not sprite
    -- get_transform(entity).position.y = get_transform(entity).position.y - fallSpeed
end
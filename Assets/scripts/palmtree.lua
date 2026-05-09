local nutDropDelayRange = {2.0, 5.5}
local nutDropDelay = 0

local firstFrame = true

function onUpdate(entity, dt)
  if firstFrame then
    firstFrame = false
    math.randomseed(os.time())
  end
  if entity == nil then
    print("Entity is nil")
    return
  end
  local transform = get_transform(entity)

  if transform == nil then
    print("Transform is nil")
    return
  end

  if nutDropDelay <= 0
  or is_key_pressed("Space") then
    nutDropDelay = nutDropDelayRange[1]
    local prefab = instantiate("coconut.prefab", Vec2.new(0, 0))

    if prefab == nil then
      print("Failed to instantiate prefab")
    else
      print("Prefab instantiated successfully")
    end
  else
    nutDropDelay = nutDropDelay - dt
  end
end
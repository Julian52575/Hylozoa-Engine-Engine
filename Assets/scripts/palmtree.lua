local nutDropDelayRange = {2.0, 5.5}
local nutDropDelay = 0
local nutDropXRange = 40
local nutDropYRange = 60

local firstFrame = true

local function dropNut(transform)
    nutDropDelay = nutDropDelayRange[math.random(1, 2)]
    local prefab = instantiate(
      "coconut.prefab.json",
      Vec2.new(
        transform.position.x + math.random(-nutDropXRange, nutDropXRange),
        transform.position.y + math.random(-nutDropYRange, 0)
      )
    )

    if prefab == nil then
      print("Failed to instantiate coconut prefab")
      return
    end
end

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
    dropNut(transform)
  else
    nutDropDelay = nutDropDelay - dt
  end
end
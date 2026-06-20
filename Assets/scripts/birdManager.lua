local birdSpawnDelayRange = {1, 12}
local birdSpawnDelay = 0
local birdSpawnXRange = {200, 300}
  local birdFollowerXRange = {40, 100}
local birdSpawnYRange = {-50, 200}
  local birdFollowerYRange = {-50, 50}
local birdFollowerRange = {1, 5} 

local firstFrame = true

local function spawnFollowers(x, y)
  for i = birdFollowerRange[1], math.random(birdFollowerRange[1], birdFollowerRange[2]) do
    local prefab = instantiate(
      "Bird",
      Vec2.new(
        x + math.random(birdFollowerXRange[1], birdFollowerXRange[2]),
        y + math.random(birdFollowerYRange[1], birdFollowerYRange[2])
      )
    )
    
    if prefab == nil then
      print("Failed to instantiate bird prefab")
      return
    end
  end -- for
end

local function spawnBird(transform)
  birdSpawnDelay = math.random(birdSpawnDelayRange[1], birdSpawnDelayRange[2]) / 10
  local x = transform.position.x + math.random(birdSpawnXRange[1], birdSpawnXRange[2])
  local y = transform.position.y + math.random(birdSpawnYRange[1], birdSpawnYRange[2])
  
  local prefab = instantiate(
      "Bird",
      Vec2.new(
        x + math.random(birdFollowerXRange[1], birdFollowerXRange[2]),
        y + math.random(birdFollowerYRange[1], birdFollowerYRange[2])
      )
    )
  spawnFollowers(x, y)
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

  if birdSpawnDelay <= 0
  or is_key_pressed("b") then
    spawnBird(transform)
  else
    birdSpawnDelay = birdSpawnDelay - dt
  end
end
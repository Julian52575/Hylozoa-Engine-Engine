local goRight = true
local lastFrameXPosition = 0
local startXPosition = 123456
local maxDistance = 240
local speed = 150

local isSupprised = false


local function move(entity, dt, transform)
  local sp = speed

  if not goRight then
    sp = sp * -1
  end

  transform.position.x = transform.position.x + sp * dt
  if transform.position.x <= startXPosition - maxDistance then
    goRight = true
    transform.rotation = 0
  end
  if transform.position.x >= startXPosition + maxDistance then
    goRight = false
    transform.rotation = math.pi
  end
end


local jumpBuffer = 0.0
local jumpSpeed = 300
local jumpDuration = 1.5
local function jump(entity, dt, transform)
  if jumpBuffer == 0 then
    jumpBuffer = jumpDuration
  end

  if jumpBuffer > 0 then
    transform.position.y = transform.position.y - jumpSpeed * dt
    jumpBuffer = jumpBuffer - dt
  else
    isSupprised = false
  end
end

--function onStart(entity)
--  print("Crab onStart")
--end

function onUpdate(entity, dt)
  if entity == nil then
    print("Entity is nil")
    return
  end
  local transform = get_transform(entity)

  if transform == nil then
    print("Transform is nil")
    return
  end
  if startXPosition == 123456 then
    startXPosition = transform.position.x
  end
  if is_mouse_button_pressed("Right") then
    log_message("Hello")
  end
  if isSupprised then
    jump(entity, dt, transform)
  else
    move(entity, dt, transform)
  end
end

function onNoise(self, source, noise)
    log_message("Noise event received: " .. noise.noiseName)
    if noise.noiseName == "coconut_fall" then
        isSupprised = true
    end
end
function onUpdate(entity, dt)
  if entity == nil then
    print("Entity is nil")
  else
    local transform = get_transform(entity)
    if transform == nil then
      print("Transform is nil")
    else
      local pos = transform.position
      pos.y = pos.y - 10 * dt
      transform.position = pos
      local new_transform = Vec2.new(0,0)
      if (is_key_pressed("Space")) then
        local prefab = instantiate("test.prefab", new_transform)
        if prefab == nil then
          print("Failed to instantiate prefab")
        else
          print("Prefab instantiated successfully")
        end
      end
    end
  end
end

function onNoise(self, source, noise)
    log_message("Noise event received: " .. noise.noiseName)
end
function onUpdate(entity, dt)
  if entity == nil then
    print("Entity is nil")
  else
    print("Updating entity:", entity)
    local transform = get_transform(entity)
    if transform == nil then
      print("Transform is nil")
    else
      local pos = transform.position
      pos.x = pos.x + 10 * dt
    end
  end
end

function onNoise(source, noise)
    log_message("Noise event received: " .. noise.noiseName)
end
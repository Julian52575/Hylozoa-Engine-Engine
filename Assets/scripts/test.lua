function onUpdate(entity, dt)
    if entity == nil then
      print("Entity is nil")
     else
      local transform = get_transform(entity)
      if transform == nil then
        print("Transform is nil")
      else
        local pos = transform.position
        pos.y = pos.y + 10 * dt
        if (is_key_pressed("Space")) then
          unload_scene("Scene 1")
          print("Scene 1 unloaded")
        end
      end
    end
end

function onNoise(source, noise)
    log_message("Noise event received: " .. noise.noiseName)
end
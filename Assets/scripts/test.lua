function onUpdate(entity, dt)
    log_message("Hello from Lua! Delta time: " .. dt)
    if entity == nil then
      print("Entity is nil")
     else
      local transform = get_transform(entity)
      if transform == nil then
        print("Transform is nil")
      else
        local pos = transform.position
        print("y:", pos.y)
        pos.y = pos.y + 10 * dt
            -- print("Entity position: (" .. transform.position.x .. ", " .. transform.position.y .. ") ")
      end
    end
end

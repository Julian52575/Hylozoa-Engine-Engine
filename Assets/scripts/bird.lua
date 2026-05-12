local speed = math.random(350, 400) + (math.random(0, 9) / 10)

function onUpdate(entity, dt)
  local position = get_transform(entity).position

  if position.x < -1000 then
    destroy_entity(entity)
  end
  position.x = position.x - speed * dt
end
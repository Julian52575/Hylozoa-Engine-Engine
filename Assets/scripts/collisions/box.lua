function onUpdate(self, dt)
end

function onNoise(self, source, noiseInfo)
end

function onCollisionBegin(self, other)

    local pos = get_transform(self).position
    log_message(pos)
    log_message("Collision detected between " .. get_name(self) .. " and " .. get_name(other))
end

function onCollisionEnd(self, other)
end

function onSensorEnter(self, other)
end

function onSensorExit(self, other)
end
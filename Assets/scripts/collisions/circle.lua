function onUpdate(self, dt)
end

function onNoise(self, source, noiseInfo)
end

function onCollisionBegin(self, other)
end

function onCollisionEnd(self, other)
end

function onSensorEnter(self, other)
    log_message("Sensor enter detected between ", get_name(self), " and ", get_name(other))
end

function onSensorExit(self, other)
    log_message("Sensor exit detected between ", get_name(self), " and ", get_name(other))
    destroy_entity(self)
end
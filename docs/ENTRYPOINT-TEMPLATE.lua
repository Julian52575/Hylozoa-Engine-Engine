--[[
* This is a template for an entry point script in the Hylozoa Game Engine.
* It show all the available entry point functions that can be implemented in a script.
]]

--[[
* Called every frame with the delta time since the last frame.
* @param self The entity that this script is attached to.
* @param dt The delta time since the last frame, in seconds.
]]
function onUpdate(self, dt)
end

--[[
* Called when a noise event is emitted and this entity has a noise listener component and within the hearing range
* @param self The entity that this script is attached to.
* @param source The entity that emitted the noise event.
* @param noiseInfo A table containing information about the noise event, such as the noise name and volume.
]]
function onNoise(self, source, noiseInfo)
end

--[[
* Called when a collision between this entity and another entity begins.    
* @param self The entity that this script is attached to.
* @param other The entity that this entity collided with.
]]
function onCollisionBegin(self, other)
end

--[[
* Called when a collision between this entity and another entity ends.
* @param self The entity that this script is attached to.
* @param other The entity that this entity stopped colliding with.
]]
function onCollisionEnd(self, other)
end

--[[
* Called when another entity enters the sensor area of this entity. 
* @param self The entity that this script is attached to.
* @param other The entity that entered the sensor area.
]]
function onSensorEnter(self, other)
end

--[[
* Called when another entity exits the sensor area of this entity.
* @param self The entity that this script is attached to.
* @param other The entity that exited the sensor area.
]]
function onSensorExit(self, other)
end
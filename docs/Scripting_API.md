# Hylozoa Lua Scripting API

A lightweight scripting API for gameplay logic in the Hylozoa Game Engine.

Scripts are written in Lua and attached to entities inside the engine.

---

# Getting Started

A basic script example:

```lua
function onUpdate(self, dt)
    local transform = get_transform(self)

    if transform and is_key_held("D") then
        transform.position.x = transform.position.x + 100 * dt
    end
end
```

---

# Script Entry Points

Entry points are optional Lua functions automatically called by the engine.

---

## onUpdate(self, dt)

Called every frame.

### Parameters

| Name | Type   | Description               |
| ---- | ------ | ------------------------- |
| self | Entity | Entity owning this script |
| dt   | number | Delta time in seconds     |

### Example

```lua
function onUpdate(self, dt)
    local transform = get_transform(self)

    if transform then
        transform.rotation = transform.rotation + 90 * dt
    end
end
```

---

## onNoise(self, source, noiseInfo)

Called when this entity hears a noise event.

Requires the entity to have a noise listener component.

### Parameters

| Name      | Type      | Description                   |
| --------- | --------- | ----------------------------- |
| self      | Entity    | Entity owning this script     |
| source    | Entity    | Entity that emitted the noise |
| noiseInfo | NoiseInfo | Information about the noise   |

### Example

```lua
function onNoise(self, source, noiseInfo)
    print("Heard noise:", noiseInfo.noiseName)
end
```

---

## onCollisionBegin(self, other)

Called when a collision starts.

### Parameters

| Name  | Type   | Description                |
| ----- | ------ | -------------------------- |
| self  | Entity | Entity owning this script  |
| other | Entity | The other colliding entity |

### Example

```lua
function onCollisionBegin(self, other)
    print("Collision with:", get_name(other))
end
```

---

## onCollisionEnd(self, other)

Called when a collision ends.

### Parameters

| Name  | Type   | Description               |
| ----- | ------ | ------------------------- |
| self  | Entity | Entity owning this script |
| other | Entity | The other entity          |

### Example

```lua
function onCollisionEnd(self, other)
    print("Stopped colliding with:", get_name(other))
end
```

---

## onSensorEnter(self, other)

Called when another entity enters this entity sensor area.

### Parameters

| Name  | Type   | Description               |
| ----- | ------ | ------------------------- |
| self  | Entity | Entity owning this script |
| other | Entity | Entering entity           |

### Example

```lua
function onSensorEnter(self, other)
    print(get_name(other), "entered sensor")
end
```

---

## onSensorExit(self, other)

Called when another entity exits this entity sensor area.

### Parameters

| Name  | Type   | Description               |
| ----- | ------ | ------------------------- |
| self  | Entity | Entity owning this script |
| other | Entity | Exiting entity            |

### Example

```lua
function onSensorExit(self, other)
    print(get_name(other), "left sensor")
end
```

---

# Utility Functions

---

## log_message(...)

Print a message in the console with "[Script-log]" prefix.

it follow the same usage as the default lua `print` function

### Exemple

```lua
log_message("Hello")
log_message("Player position: ", 15, 20)
```

---

# Entity API

---

## get_transform(entity)

Get the transform component of an entity.

### Parameters

| Name   | Type   |
| ------ | ------ |
| entity | Entity |

### Returns

| Type      | Description            |
| --------- | ---------------------- |
| Transform | Entity transform       |
| nil       | If no transform exists |

### Example

```lua
local transform = get_transform(self)

if transform then
    transform.position.x = 100
end
```

---

## get_name(entity)

Get the name of an entity.

### Parameters

| Name   | Type   |
| ------ | ------ |
| entity | Entity |

### Returns

| Type   | Description |
| ------ | ----------- |
| string | Entity name |

### Example

```lua
local name = get_name(self)
print(name)
```

---

## destroy_entity(entity)

Destroy an entity and remove it from the scene.

### Parameters

| Name   | Type   |
| ------ | ------ |
| entity | Entity |

### Example

```lua
destroy_entity(self)
```

---

## instantiate(prefabPath, position)

Instantiate a prefab in the world.

Prefab paths are relative to the `Assets/` directory.

### Parameters

| Name       | Type   | Description    |
| ---------- | ------ | -------------- |
| prefabPath | string | Path to prefab |
| position   | Vec2   | World position |

### Returns

| Type   | Description    |
| ------ | -------------- |
| Entity | Spawned entity |
| nil    | Spawn failed   |

### Example

```lua
local enemy = instantiate(
    "Prefabs/enemy.prefab",
    Vec2.new(10, 5)
)
```

---

# Input API

---

## is_key_pressed(key)

Returns true only on the frame the key is pressed.

### Example

```lua
if is_key_pressed("Space") then
    print("Jump")
end
```

---

## is_key_released(key)

Returns true only on the frame the key is released.

### Example

```lua
if is_key_released("E") then
    print("Released E")
end
```

---

## is_key_held(key)

Returns true while the key remains held down.

### Example

```lua
if is_key_held("A") then
    print("Moving left")
end
```

### Common Keys

| Key        |
| ---------- |
| `"W"`      |
| `"A"`      |
| `"S"`      |
| `"D"`      |
| `"Space"`  |
| `"Escape"` |
| `"LShift"` |

---

## is_mouse_button_pressed(button)

Returns true only on the frame the button is pressed.

### Example

```lua
if is_mouse_button_pressed("Left") then
    print("left click")
end
```

---

## is_mouse_button_released(button)

Returns true only on the frame the button is pressed.

### Example

```lua
if is_mouse_button_released("Left") then
    print("left released")
end
```

### Common Mouse Button

| Button     |
| ---------- |
| `"Left"`   |
| `"Right"`  |
| `"Middle"` |
| `"X1"`     |
| `"X2"`     |


# Scene API

---

## load_scene(sceneName)

Load a scene by name.

### Parameters

| Name      | Type   |
| --------- | ------ |
| sceneName | string |

### Example

```lua
load_scene("MainMenu")
```

---

## unload_scene(sceneName)

Unload a scene by name.

### Parameters

| Name      | Type   |
| --------- | ------ |
| sceneName | string |

### Example

```lua
unload_scene("CombatScene")
```

---

## has_tag(entity, tag)

Check if an entity has a specified tag

### Parameters

| Name      | Type    |
| --------- | ------  |
| entity    | Entity  |
| tag       | Tags.tag|

### Returns


| Value     | Description                                                                       |
| --------- | -------------------------                                                         |
| true      | entity have the specified tag                                                     |
| false     | entity do not have the specified tag or an error occured (e.g. Tag not registered/do not exist or the entity do not have a `Tags` components)                                     |


### Exemple

The `Tags` namespace is a global namespace that hold all or registered tag in the current project. For exemple if the project have registered he tags `Enemy` you would access it like so:

```lua
if has_tag(entity, Tags.Enemy) then
  --do something
end
```

---

## add_tag(entity, tag)

add the specified tag to the target entity

### Parameters

| Name      | Type    |
| --------- | ------  |
| entity    | Entity  |
| tag       | Tags.tag|

### Returns


| Value     | Description                                                                       |
| --------- | -------------------------                                                         |
| true      | specified tag added succesfully                                                   |
| false     | an error occured (e.g. Tag not registered/do not exist, the entity do not have a `Tags` components or entity already have specified tag)                                     |


### Exemple

The `Tags` namespace is a global namespace that hold all or registered tag in the current project. For exemple if the project have registered he tags `Enemy` you would add it like so:

```lua
add_tag(entity, Tags.Enemy)
```

>[note]
>this `add_tag` function do not register new tags nor does the `remove_tag` function unregister tags. Tags are registered on project configurations and any modification need an engine settings reload.

---

## remove_tag(entity, tag)

remoev the specified tag to the target entity

### Parameters

| Name      | Type    |
| --------- | ------  |
| entity    | Entity  |
| tag       | Tags.tag|

### Returns


| Value     | Description                                                                       |
| --------- | -------------------------                                                         |
| true      | specified tag removed successfully                                                |
| false     | an error occured (e.g. Tag not registered/do not exist, the entity do not have a `Tags` components or entity do not have specified tag)                                     |


### Exemple

The `Tags` namespace is a global namespace that hold all or registered tag in the current project. For exemple if the project have registered he tags `Enemy` you would remove it like so:

```lua
remove_tag(entity, Tags.Enemy)
```

>[note]
>this `add_tag` function do not register new tags nor does the `remove_tag` function unregister tags. Tags are registered on project configurations and any modification need an engine settings reload.
---

# Lua User Types

---

# Vec2

2D vector type.

## Constructor

```lua
Vec2.new(x, y)
```

## Fields

| Field | Type   |
| ----- | ------ |
| x     | number |
| y     | number |

### Example

```lua
local direction = Vec2.new(1, 0)

print(direction.x)
```

---

# Transform

Entity transform component.

## Fields

| Field    | Type   |
| -------- | ------ |
| position | Vec2   |
| rotation | number |
| scale    | Vec2   |

### Example

```lua
local transform = get_transform(self)

transform.position.x = transform.position.x + 10
transform.rotation = 45
```

---

# Entity

Represents an entity inside the world.

Currently acts as an opaque engine object used by the scripting API.

---

# NoiseInfo

Information about a noise event.

## Fields

| Field     | Type   |
| --------- | ------ |
| noiseName | string |
| position  | Vec2   |
| direction | Vec2   |

### Example

```lua
function onNoise(self, source, noiseInfo)
    print(noiseInfo.noiseName)

    print(noiseInfo.position.x)
    print(noiseInfo.position.y)
end
```

---

# Complete Example

```lua
function onUpdate(self, dt)
    local transform = get_transform(self)

    if not transform then
        return
    end

    local speed = 100

    if is_key_held("W") then
        transform.position.y =
            transform.position.y - speed * dt
    end

    if is_key_held("S") then
        transform.position.y =
            transform.position.y + speed * dt
    end

    if is_key_held("A") then
        transform.position.x =
            transform.position.x - speed * dt
    end

    if is_key_held("D") then
        transform.position.x =
            transform.position.x + speed * dt
    end
end
```

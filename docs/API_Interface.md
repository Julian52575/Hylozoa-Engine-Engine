# Hylozoa Engine UI API

A lightweight C API for controlling the Hylozoa Game Engine from the UI.


---

# Getting Started

The API manages a single global engine instance.

A basic lifecycle example:

```cpp
engine_create(settings, isRaw);
engine_init();
engine_run();
engine_stop();
engine_shutdown();
```

Call `engine_create` before using scene, layer, settings, or runtime functions.

When a function has an `isRaw` parameter:

| Value | Meaning                                |
| ----- | -------------------------------------- |
| true  | The string parameter contains raw JSON |
| false | The string parameter contains a path   |

Most functions do nothing, return `false`, or return `nullptr` when the engine
has not been created.

Functions that catch errors print an `[API-CATCH]` message to `stderr`.

---

# Engine API

---

## get_engine_instance()

Get the current global engine instance.

### Returns

| Type              | Description                                      |
| ----------------- | ------------------------------------------------ |
| Hylozoa::Engine * | Current engine instance                          |
| nullptr           | If no engine exists or after `engine_shutdown()` |

---

## engine_create(settings, isRaw)

Create the global engine instance.

This function must be called before `engine_init`, `engine_run`, scene
management, layer management, or settings reloads.

If an engine instance already exists, this function returns without replacing
it.

### Parameters

| Name     | Type        | Description                                      |
| -------- | ----------- | ------------------------------------------------ |
| settings | const char* | Settings JSON, settings file path, or `nullptr`  |
| isRaw    | bool        | `true` for raw JSON, `false` for a file path     |

### Behavior

| Input                         | Result                                                |
| ----------------------------- | ----------------------------------------------------- |
| `settings == nullptr`         | Create the engine with default normal-mode settings   |
| `settings != nullptr`, raw    | Read settings from the JSON string                    |
| `settings != nullptr`, file   | Read settings from the provided file path             |
| Engine already exists         | Do nothing                                            |

---

## engine_init()

Initialize the engine.

This sets up the engine managers and systems.

### Notes

| Case             | Behavior        |
| ---------------- | --------------- |
| Engine exists    | Calls `init()`  |
| No engine exists | Does nothing    |

---

## engine_run()

Run the engine main loop.

While running, the engine can process events, update scenes and systems, and
render output.

### Notes

| Case             | Behavior       |
| ---------------- | -------------- |
| Engine exists    | Calls `run()`  |
| No engine exists | Does nothing   |

---

## engine_pause()

Pause the engine.

Use this to temporarily halt normal engine activity without destroying the
engine instance.

### Notes

| Case             | Behavior        |
| ---------------- | --------------- |
| Engine exists    | Calls `pause()` |
| No engine exists | Does nothing    |

---

## engine_unpause()

Unpause the engine.

Use this to resume engine activity after `engine_pause`.

### Notes

| Case             | Behavior          |
| ---------------- | ----------------- |
| Engine exists    | Calls `unpause()` |
| No engine exists | Does nothing      |

---

## engine_stop()

Stop the engine loop.

This halts engine execution without deleting the engine instance.

### Notes

| Case             | Behavior       |
| ---------------- | -------------- |
| Engine exists    | Calls `stop()` |
| No engine exists | Does nothing   |

---

## engine_shutdown()

Shut down the engine and release engine resources.

This stops the engine, shuts it down, deletes the global instance, clears the
global pointer, and shuts down the SDL manager.

### Notes

| Case             | Behavior                              |
| ---------------- | ------------------------------------- |
| Engine exists    | Stop, shutdown, delete, clear pointer |
| No engine exists | Does nothing                          |

---

# Scene API

---

## scene_create(sceneData, isRaw)

Create and register a scene from JSON data.

The scene JSON must match the format expected by the engine scene serializer.

### Parameters

| Name      | Type        | Description                                  |
| --------- | ----------- | -------------------------------------------- |
| sceneData | const char* | Raw scene JSON or path to a scene JSON file  |
| isRaw     | bool        | `true` for raw JSON, `false` for a file path |

### Returns

| Value | Description                                                                          |
| ----- | ------------------------------------------------------------------------------------ |
| true  | Scene JSON was parsed and deserialized into an existing engine                       |
| false | Null data, missing file, parse error, deserialization error, or no engine instance   |

---

## scene_destroy_uuid(sceneId)

Destroy a registered scene by UUID.

### Parameters

| Name    | Type     | Description                  |
| ------- | -------- | ---------------------------- |
| sceneId | uint64_t | UUID of the scene to destroy |

### Returns

| Value | Description                                      |
| ----- | ------------------------------------------------ |
| true  | successfully destroyed the scene                 |
| false | an Error occured                                 |


---

## scene_destroy_name(sceneName)

Destroy a registered scene by name.

### Parameters

| Name      | Type        | Description                  |
| --------- | ----------- | ---------------------------- |
| sceneName | const char* | Name of the scene to destroy |

### Returns

| Value | Description                                      |
| ----- | ------------------------------------------------ |
| true  | successfully destroyed the scene                 |
| false | an Error occured                                 |


---

## scene_destroy(scene, isUUID)

Destroy a registered scene by either UUID string or name.

### Parameters

| Name   | Type        | Description                                              |
| ------ | ----------- | -------------------------------------------------------- |
| scene  | const char* | Scene UUID as decimal text, or scene name                |
| isUUID | bool        | `true` to parse `scene` as a UUID, `false` to use a name |

### Returns

| Value | Description                                                        |
| ----- | ------------------------------------------------------------------ |
| true  | successfully destroyed the scene                                  |
| false | UUID parsing failed, or an error occured in destroying the scene   |

>[note]
>When `isUUID` is `true`, the UUID is parsed with `std::stoull`, so the string
>must contain a decimal numeric UUID.

---

## scene_load_uuid(sceneId)

Load a scene by UUID.

Loading a scene makes it active for update and rendering.

### Parameters

| Name    | Type     | Description               |
| ------- | -------- | ------------------------- |
| sceneId | uint64_t | UUID of the scene to load |

### Returns

| Value | Description                                      |
| ----- | ------------------------------------------------ |
| true  | successfully loaded the scene                    |
| false | an error occured                                 |


---

## scene_load_name(sceneName)

Load a scene by name.

Loading a scene makes it active for update and rendering.

### Parameters

| Name      | Type        | Description               |
| --------- | ----------- | ------------------------- |
| sceneName | const char* | Name of the scene to load |

### Returns

| Value | Description                                      |
| ----- | ------------------------------------------------ |
| true  | successfully loaded the scene                    |
| false | an error occured                                 |

---

## scene_load(scene, isUUID)

Load a scene by either UUID string or name.

### Parameters

| Name   | Type        | Description                                              |
| ------ | ----------- | -------------------------------------------------------- |
| scene  | const char* | Scene UUID as decimal text, or scene name                |
| isUUID | bool        | `true` to parse `scene` as a UUID, `false` to use a name |

### Returns

| Value | Description                                                   |
| ----- | ------------------------------------------------------------- |
| true  | successfully loaded the scene                                 |
| false | UUID parsing failed, or an error occured in loading the scene |

>[note]
>When `isUUID` is `true`, the UUID is parsed with `std::stoull`, so the string
>must contain a decimal numeric UUID.

---

## scene_unload_uuid(sceneId)

Unload a scene by UUID.

Unloading deactivates the scene so it is no longer updated or rendered.

### Parameters

| Name    | Type     | Description                 |
| ------- | -------- | --------------------------- |
| sceneId | uint64_t | UUID of the scene to unload |

### Returns

| Value | Description                                      |
| ----- | ------------------------------------------------ |
| true  | successfully unloaded the scene                  |
| false | an error occured                                 |


---

## scene_unload_name(sceneName)

Unload a scene by name.

Unloading deactivates the scene so it is no longer updated or rendered.

### Parameters

| Name      | Type        | Description                 |
| --------- | ----------- | --------------------------- |
| sceneName | const char* | Name of the scene to unload |

### Returns

| Value | Description                                      |
| ----- | ------------------------------------------------ |
| true  | successfully unloaded the scene                  |
| false | an error occured                                 |

---

## scene_unload(scene, isUUID)

Unload a scene by either UUID string or name.

### Parameters

| Name   | Type        | Description                                              |
| ------ | ----------- | -------------------------------------------------------- |
| scene  | const char* | Scene UUID as decimal text, or scene name                |
| isUUID | bool        | `true` to parse `scene` as a UUID, `false` to use a name |

### Returns

| Value | Description                                                 |
| ----- | ----------------------------------------------------------- |
| true  | successfully unloaded the scene                             |
| false | an error occured                                            |

>[note]
>When `isUUID` is `true`, the UUID is parsed with `std::stoull`, so the string
>must contain a decimal numeric UUID.

---

## scene_list()

Return the registered scene list as a JSON string.

Expected output from the header comments is a JSON string describing all
registered scenes, including loaded and unloaded scenes.

### Returns

| Type        | Description                      |
| ----------- | -------------------------------- |
| const char* | JSON string containing scenes    |

>[note]
>`scene_list` is not implemented yet

---

# Layer API

---

## layer_create(layerName)

Register a new layer in the engine layer manager.

### Parameters

| Name      | Type        | Description                   |
| --------- | ----------- | ----------------------------- |
| layerName | const char* | Name of the layer to register |

### Notes

| Case             | Behavior                                |
| ---------------- | --------------------------------------- |
| Engine exists    | Calls `registerLayer(layerName)`        |
| No engine exists | Does nothing                            |
| Error occurs     | Prints an `[API-CATCH]` message         |

---

## layer_destroy(layerName)

Unregister an existing layer from the engine layer manager.

### Parameters

| Name      | Type        | Description                     |
| --------- | ----------- | ------------------------------- |
| layerName | const char* | Name of the layer to unregister |

### Notes

| Case             | Behavior                                |
| ---------------- | --------------------------------------- |
| Engine exists    | Calls `unregisterLayer(layerName)`      |
| No engine exists | Does nothing                            |
| Error occurs     | Prints an `[API-CATCH]` message         |

---

## layer_list()

Return the currently registered layer names as a JSON array string.

### Returns

| Type        | Description                                  |
| ----------- | -------------------------------------------- |
| const char* | JSON array string allocated by the engine    |
| nullptr     | No engine exists or an error occurred        |

### Example

```json
["Default","UI","Debug"]
```

>[note]
>The returned string is allocated by the engine with `new[]`.
>Release it with `free_string`.

---

## free_string(str)

Free a string allocated by the engine API.

Use this for strings returned by functions such as `layer_list`.

### Parameters

| Name | Type        | Description                         |
| ---- | ----------- | ----------------------------------- |
| str  | const char* | Engine-allocated string to release |

>[note]
>Passing `nullptr` is safe with the current implementation because
>`delete[] nullptr` is valid in C++.

---

# Utility API

---

## engine_version()

Return the engine version string.

### Returns

| Type        | Description           |
| ----------- | --------------------- |
| const char* | Engine version string |

>[note]
>`engine_version` is not impemented yet

---

## generate_uuid(outPtr, size)

Generate a new UUID and write it as a null-terminated decimal string.

### Parameters

| Name   | Type   | Description                         |
| ------ | ------ | ----------------------------------- |
| outPtr | char*  | Caller-owned output buffer          |
| size   | size_t | Output buffer size in bytes         |

### Notes

| Case                         | Behavior                                      |
| ---------------------------- | --------------------------------------------- |
| `outPtr == nullptr`          | Do nothing                                    |
| `size == 0`                  | Do nothing                                    |
| Valid buffer                 | Write at most `size - 1` characters plus `\0` |

>[note]
>A buffer of at least 21 bytes is recommended for any `uint64_t` UUID value plus
>the null terminator.

---

## reload_settings_file(settingsPath)

Reload engine settings from a JSON file.

### Parameters

| Name         | Type        | Description                    |
| ------------ | ----------- | ------------------------------ |
| settingsPath | const char* | Path to the JSON settings file |

### Notes

| Case             | Behavior                        |
| ---------------- | ------------------------------- |
| Engine exists    | Calls `reloadSettings(path)`    |
| No engine exists | Does nothing                    |
| Error occurs     | Prints an `[API-CATCH]` message |

---

## reload_settings_json(settingsJson)

Reload engine settings from raw JSON.

### Parameters

| Name         | Type        | Description              |
| ------------ | ----------- | ------------------------ |
| settingsJson | const char* | Raw JSON settings string |

### Notes

| Case             | Behavior                         |
| ---------------- | -------------------------------- |
| Engine exists    | Parses JSON and reloads settings |
| No engine exists | Does nothing                     |
| Error occurs     | Prints an `[API-CATCH]` message  |

---

## reload_settings(settings, isRaw)

Reload engine settings from either a file path or raw JSON.

### Parameters

| Name     | Type        | Description                                  |
| -------- | ----------- | -------------------------------------------- |
| settings | const char* | Raw JSON settings or a settings file path    |
| isRaw    | bool        | `true` for raw JSON, `false` for a file path |

### Behavior

| isRaw | Called function                    |
| ----- | ---------------------------------- |
| true  | `reload_settings_json(settings)`   |
| false | `reload_settings_file(settings)`   |

---

# Integration Notes

- Create the engine once. Calling `engine_create` again while an engine already
  exists does not replace the existing instance.
- Shut down with `engine_shutdown` before unloading the dynamic library or
  closing the UI.
- Any string returned by the engine API and documented as engine-allocated
  should be released with `free_string`.
- The generic scene helpers (`scene_load`, `scene_unload`, `scene_destroy`)
  parse UUIDs with `std::stoull`, so UUID strings must be decimal numeric text.

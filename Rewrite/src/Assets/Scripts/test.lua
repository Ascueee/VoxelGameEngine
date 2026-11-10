--test script which moves camera above sea level
local entity = ...

--entity.transform.position = entity.transform.position + Vector3.new(0, 0, 0.1)

entity.camera.position = entity.transform.position + Vector3.new(0, 0, 20);
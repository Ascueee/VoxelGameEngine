local entityManager = EntityManager.new()
local inputManager = InputSystem.new()
local modelStorage = ModelStorage.new()
local speed = 1;

function Load(entity)
    print "[Script]Loading the script..."

    --modifies transform package
    entity.transform.position = Vector3.new(1000, 400, 30)
    entity.transform.scale = Vector3.new(1, 1, 1)
    entity.transform.rotation = Quaternion.new(1, 0, 0, 0)

    --sets entity model
    entity.meshController.model = modelStorage:GetModel("Monkey")
end

function Update(entity)
    EntityMovement(entity)
    EntityCamera(entity)

    --entity.transform.rotation = entity.transform.rotation + Quaternion.new(1, 0, 0, 0.1);
    --updates camera position with an offset
    entity.camera.position = entity.transform.position + Vector3.new(0, 0, 10); 
end

function EntityMovement(entity)
    if inputManager:IsKeyPressed(Keys.W) then
        entity.transform.position = entity.transform.position + Vector3.new(0, 0, speed)
    end

    if inputManager:IsKeyPressed(Keys.S) then
        entity.transform.position = entity.transform.position + Vector3.new(0, 0, -speed)
    end
end

function EntityCamera(entity)
    --print("mouse X: ", inputManager.mouseX)
end
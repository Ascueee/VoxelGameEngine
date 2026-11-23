local entityManager = EntityManager.new()
local inputManager = InputSystem.new()
local modelStorage = ModelStorage.new()
local textureStorage = TextureStorage.new()
local speed = 1;

function Load(entity)
    print "[Script]Loading the script..."

    --modifies transform package
    entity.transform.position = Vector3.new(1000, 400, 30)
    entity.transform.scale = Vector3.new(0.01, 0.01, 0.01)
    entity.transform.rotation = Quaternion.new(1, 0, 0, 0)

    --sets entity model
    entity.modelController.model = modelStorage:GetModel("PlayerModel")
    entity.modelController.model:GetMesh("Ch36").texture = textureStorage:GetTexture("PlayerSkin")
    --creates a material because base material is empty
end

function Update(entity)
    EntityMovement(entity)
    EntityCamera(entity)

    --entity.transform.rotation = entity.transform.rotation + Quaternion.new(1, 0, 0, 0.1);
    --updates camera position with an offset
    entity.camera.position = entity.transform.position + Vector3.new(0, 0, 5); 
end

function EntityMovement(entity)
    if inputManager:IsKeyPressed(Keys.W) then
        entity.transform.position = entity.transform.position + Vector3.new(0, 0, speed)
    end

    if inputManager:IsKeyPressed(Keys.S) then
        entity.transform.position = entity.transform.position + Vector3.new(0, 0, -speed)
    end

    if inputManager:IsKeyPressed(Keys.A) then
        entity.transform.position = entity.transform.position + Vector3.new(-speed, 0, 0)
    end

    if inputManager:IsKeyPressed(Keys.D) then
        entity.transform.position = entity.transform.position + Vector3.new(speed, 0, 0)
    end
end

function EntityCamera(entity)
    --print("mouse X: ", inputManager.mouseX)
end
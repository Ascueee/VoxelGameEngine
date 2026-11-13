local entityManager = EntityManager.new()
local modelStorage = ModelStorage.new()

function Load(entity)
    print "[Script]Loading the script..."

    entity.transform.position = Vector3.new(1000,400,30)
    entity.meshController.model = modelStorage:GetModel("Monkey")
end

function Update(entity)
    --entity.transform.position = entity.transform.position + Vector3.new(0, 0, 0.1)

    entity.camera.position = entity.transform.position + Vector3.new(0, 0, 10); 
end

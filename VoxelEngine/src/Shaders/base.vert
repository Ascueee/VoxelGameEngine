#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 boneIds;
layout (location = 4) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    bool hasBoneInfluence = false;

    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        int boneId = int(boneIds[i]);
        if (boneId < 0)
            continue;
        if (boneId >= MAX_BONES) {
            totalPosition = vec4(aPos, 1.0f);
            hasBoneInfluence = true;
            break;
        }
        totalPosition += (finalBonesMatrices[boneId] * vec4(aPos, 1.0f)) * weights[i];
        hasBoneInfluence = true;
    }

    if (!hasBoneInfluence)
        totalPosition = vec4(aPos, 1.0f);

    texCoord = aTexCoord;
    normal = mat3(transpose(inverse(model))) * aNormal; 
    fragPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * model * totalPosition;
}
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 boneIds;
layout (location = 2) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

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

    gl_Position = projection * view * model * totalPosition;
}
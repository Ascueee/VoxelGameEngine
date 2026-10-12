#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 uv;
out vec3 FragPos;
out vec3 normal;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);

    FragPos = worldPos.xyz;
    uv = aUV;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    normal = normalMatrix * aNormal;
    
    gl_Position = projection * view * worldPos;
}
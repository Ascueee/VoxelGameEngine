
#version 330 core

layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gDiffuse;
layout (location = 2) out vec3 gNormal;

in vec2 uv;
in vec3 FragPos;
in vec3 normal;

uniform sampler2D diffuse;

void main()
{
    gPos = FragPos;
    gDiffuse = texture(diffuse, uv).rgb;
    gNormal = normal;
}

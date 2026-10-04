#version 330 core

uniform vec3 color;
uniform sampler2D diffuse;

in vec2 uv;
out vec4 FragColor;

void main(){
    FragColor = texture(diffuse, uv) * vec4(color, 1.0f);
}
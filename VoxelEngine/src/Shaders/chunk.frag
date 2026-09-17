#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform vec3 color;
uniform sampler2D diffuseTexture;
void main(){
    FragColor = texture(diffuseTexture, texCoord) * vec4(color, 1.0f);
}
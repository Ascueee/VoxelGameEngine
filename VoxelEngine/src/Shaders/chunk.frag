#version 330 core
out vec4 FragColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 color;
uniform sampler2D diffuseTexture;
uniform vec3 lightPos;
uniform vec3 lightColor; 

void main(){

    //Ambient Lighting
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * color;
    FragColor = texture(diffuseTexture, texCoord) * vec4(result, 1.0f);
}
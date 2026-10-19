#version 330 core
uniform sampler2D diffuse;

in vec2 uv;
out vec4 FragColor;

void main(){
    vec4 texColor = texture(diffuse, uv);
    if(texColor.a < 0.5) discard;
    FragColor = vec4(texColor.rgb, 1.0);
    //FragColor = vec4(uv, 0.0, 1.0);
}
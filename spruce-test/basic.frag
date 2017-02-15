#version 400 core

in vec3 pass_color;
in vec2 pass_uv;

out vec3 color;

uniform sampler2D uTexture;

void main() { 
    color = texture(uTexture, pass_uv).rgb;
}
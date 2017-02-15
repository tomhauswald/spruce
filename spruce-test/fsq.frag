#version 400 core

in vec2 pass_uv;

out vec4 color;

uniform sampler2D uPositionTexture;
uniform sampler2D uNormalTexture;
uniform sampler2D uAlbedoTexture;

void main() { 
    color = vec4(texture(uAlbedoTexture, pass_uv).rgb, 1.0f);
}
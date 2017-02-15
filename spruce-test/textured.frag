#version 400 core

in vec3 pass_position;
in vec3 pass_normal;
in vec3 pass_color;
in vec2 pass_uv;

layout (location = 0) out vec4 position;
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 albedo;

uniform sampler2D uTexture;

void main() { 
    position = vec4(pass_position, 1.0f);
    normal   = vec4(pass_normal, 0.0f); 
    albedo   = vec4(pass_color * texture(uTexture, pass_uv).rgb, 1.0f);
}
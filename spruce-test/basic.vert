#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 uv;

out vec3 pass_color;

uniform mat4x4    uWorldViewProjection;
uniform sampler2D uTexture;

void main() {
  gl_Position   = uWorldViewProjection * vec4(position, 1.0f);
  gl_Position.w = 1.0f;
  pass_color    = texture(uTexture, uv).rgb;
}
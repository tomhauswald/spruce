#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 uv;

out vec3 pass_color;

void main() {
  gl_Position.xyz = position;
  gl_Position.w = 1.0;
  pass_color = color;
}
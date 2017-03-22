#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uv;

out vec3 pass_position;
out vec3 pass_normal;
out vec3 pass_color;
out vec2 pass_uv;

uniform mat4x4 uMVP;

void main() {
  gl_Position   = uMVP * vec4(position, 1.0f);
  pass_position = gl_Position.xyz;
  pass_normal   = (uMVP * vec4(normal, 0.0f)).xyz;
  pass_color    = color;
  pass_uv       = uv;
}
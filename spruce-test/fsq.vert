#version 400 core

layout(location = 0) in vec2 position;

out vec2 pass_uv;

void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
    pass_uv = vec2(
        (position.x + 1.0f) / 2.0f,
        (position.y + 1.0f) / 2.0f 
    );
}
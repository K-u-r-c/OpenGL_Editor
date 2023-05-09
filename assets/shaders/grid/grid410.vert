#version 410 core

layout (location = 0) out vec2 uv;
layout (location = 1) out vec2 camPos;

uniform mat4 view;
uniform mat4 proj;
uniform vec3 cameraPos;
uniform float gridSize;

const vec3 pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),
    vec3( 1.0, 0.0, -1.0),
    vec3( 1.0, 0.0,  1.0),
    vec3(-1.0, 0.0,  1.0)
);

const int indices[6] = int[6](
    0, 1, 2, 2, 3, 0
);

void main() {
    mat4 MVP = proj * view;

    int idx = indices[gl_VertexID];vec3 position = pos[idx] * gridSize;

    position.x += cameraPos.x;
    position.z += cameraPos.z;

    camPos = cameraPos.xz;

    gl_Position = MVP * vec4(position, 1.0);
    uv = position.xz;
}
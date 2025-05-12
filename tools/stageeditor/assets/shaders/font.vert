#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 coord;

layout(std140, binding = 0) uniform Stuff
{
    // Camera
    mat4 view;
    vec2 viewport;
    // Cells
    int cellWidth;
    int cellHeight;
    int gridWidth;
    int gridHeight;
};

out vec2 uv;

void main()
{
	uv = coord;
	gl_Position = view * vec4(pos.x / cellWidth, pos.y / cellHeight, pos.z, 1.0);
}
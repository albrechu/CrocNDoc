#version 450

layout(location = 0) in vec3 pos;

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

out flat ivec2 uv;
out float enabler;

void main()
{
    int id = gl_InstanceID;
    uv = ivec2(id % gridWidth, id / gridWidth);
    gl_Position = view * vec4(pos.xy + vec2(uv), 0.0, 1.0);
    enabler = pos.z;
}

#version 450 core

layout(location = 0) in vec2 pos;

layout(std140, binding = 0) uniform Stuff
{
    // Camera
    mat4 view; 
    vec2 viewport;
};

void main()
{
    gl_Position = view * vec4(pos, 0.0, 1.0);
}

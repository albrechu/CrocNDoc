#version 450

layout(location = 0) in vec2 pos;

out vec2 coordinate;

layout(std140, binding = 0) uniform Stuff
{
    vec3  iResolution; // viewport resolution (in pixels)
    float iTime;       // shader playback time (in seconds)
    float iTimeDelta;  // render time (in seconds)
    float iFrameRate;  // shader frame rate
    int   iFrame;      // shader playback frame
};


void main()
{
    gl_PointSize = 5.0; 
    gl_Position = vec4(((pos / 64.0) - 0.5), 0.0, 1.0);
}
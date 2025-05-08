#version 450 

in vec2 coordinate;
out vec4 fragColor;
uniform sampler2D image;

layout(std140, binding = 0) uniform Stuff
{
    vec3  iResolution; // viewport resolution (in pixels)
    float iTime;       // shader playback time (in seconds)
    float iTimeDelta;  // render time (in seconds)
    float iFrameRate;  // shader frame rate
    int   iFrame;      // shader playback frame
};

uniform float threshold = 0.05;

void main()
{
    vec2 uv    = gl_FragCoord.xy / iResolution.xy;
    vec2 uvs[8] = 
    {
        vec2(-1, -1), vec2(-1,  0), vec2(-1, 1),
        vec2( 0, -1),               vec2( 0, 1), 
        vec2( 1, -1), vec2( 1,  0), vec2( 1, 1), 
    };
    int k = 0;
    for (int i = 0; i < 8; ++i)
    {
        uvs[i] = uv + uvs[i] / iResolution.xy;
        k += int(texture(image, uvs[i]).r > threshold);
    }
    int t = 0;
    for (int i = 0; i < 8; ++i)
    {
        int next = (i + 1) % 8;
        t += int((texture(image, uvs[i]).r < threshold) && (texture(image, uvs[next]).r > threshold));
    }
    fragColor = float(!((k >= 2 && k <= 6) && (t == 1))) * texture(image, uv);
}
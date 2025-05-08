#version 450 core

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

uniform float threshold = 3.0;

void main()
{
    mat3 sobel[2];
    sobel[0] = mat3(
        -1,-2,-1,
         0, 0, 0,
         1, 2, 1
    );
    sobel[1] = mat3(
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    );

    vec2 uv    = gl_FragCoord.xy / iResolution.xy;
    vec2 value = vec2(0);

    for (int mat_id = 0; mat_id < 2; mat_id++) 
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
              vec2  offset = vec2(i - 1, j - 1) / iResolution.xy;
              float kernel = sobel[mat_id][i][j];
              vec4  color = texture(image, uv + offset);
              float luma = dot(color.rgb, vec3(0.299, 0.587, 0.114));

              value[mat_id] += luma * kernel;
            }
        }
    }
	vec4 color = step(threshold, length(value)) * vec4(vec3(length(value)), 1.0);
    fragColor = color;
}
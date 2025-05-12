#version 450 core

uniform sampler2D atlas;

out vec4 fragColor;
in vec2 uv;

void main()
{
	fragColor = vec4(texture(atlas, uv).r);
}
#version 450 core

in vec2 coordinate;
out vec4 fragColor;
uniform sampler2D image;

void main()
{
	fragColor = texture(image, coordinate);
}
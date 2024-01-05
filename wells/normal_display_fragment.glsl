#version 330 core
precision highp float;	// normal floats, makes no difference on desktop computers

uniform vec4 color;

out vec4 outColor;		// computed color of the current pixel

void main()
{
    outColor = color;
}  
#version 330 core

in vec2 vsTexCoords;
out vec4 fColor;

uniform sampler2D samplerDiffuse1;

void main()
{
	fColor = texture(samplerDiffuse1, vsTexCoords);
}
#version 430

in vec2 g_uv;
in vec4 g_color;

layout (location = 0) out vec4 fragColor;

void main()
{
	float d = 1.0 - clamp(length(g_uv), 0.0, 1.0); // d: distance 
	fragColor = d * g_color;
}
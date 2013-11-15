#version 430

in vec2 g_uv;
in vec3 g_color;

layout (location = 0) out vec4 fragColor;

void main()
{
	float d = 1.0 - length(g_uv); // d: distance 
	fragColor = vec4(g_color * d, d);
}
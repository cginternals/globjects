#version 330

in vec2 g_uv;

layout (location = 0) out vec4 fragColor;

void main()
{
	float d = 1.0 - length(g_uv); // d: distance 
	fragColor = vec4(vec3(d), d);
}
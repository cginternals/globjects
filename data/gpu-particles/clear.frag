#version 430

layout (location = 0) out vec4 fragColor;
in vec2 v_uv;

void main()
{
	fragColor = vec4(vec3(0.1), 0.0);
}
#version 430

layout (location = 0) in vec4 a_vertex;

out float v_scale;

void main()
{
	v_scale = 0.02;
	gl_Position = a_vertex;
}

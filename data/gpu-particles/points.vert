#version 430

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec4 a_velocity;

out float v_scale;
out vec3 v_color;

void main()
{
	v_scale = 0.02;
	v_color = normalize(a_velocity.xyz) * 0.5 + 0.5;
	gl_Position = a_vertex;
}

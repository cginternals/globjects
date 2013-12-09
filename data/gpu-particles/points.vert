#version 330

uniform mat4 viewProjection;

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec4 a_velocity;

out float v_scale;
out vec4 v_color;

void main()
{
	v_scale = 0.006;
	v_color = vec4(normalize(a_velocity.xyz) * 0.5 + 0.5, 0.02);
	gl_Position = viewProjection * a_vertex;
}

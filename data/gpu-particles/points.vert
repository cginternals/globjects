#version 330

layout (location = 0) in vec3 a_vertex;

out float v_scale;

void main()
{
	v_scale = 0.1;
	gl_Position = vec4(a_vertex, 1.0);
}

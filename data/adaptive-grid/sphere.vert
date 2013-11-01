#version 330

uniform mat4 transform;

layout (location = 0) in vec3 a_vertex;
//layout (location = 1) in vec3 a_normal;

flat out vec3 v_normal;

void main()
{
	v_normal = a_vertex;
	gl_Position = transform * vec4(a_vertex, 1.0);
}

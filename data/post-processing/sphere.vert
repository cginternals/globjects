#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 transform;
uniform float timef;

layout (location = 0) in vec3 a_vertex;
//layout (location = 1) in vec3 a_normal;

smooth out vec3 v_normal;
smooth out vec3 v_vertex;

const float PI2 = 6.283185;

void main()
{
	v_normal = a_vertex;
	v_vertex = a_vertex + vec3(cos(timef * PI2), 0.0, sin(timef * PI2)) * 2.0;
	gl_Position = transform * vec4(v_vertex, 1.0);
}

#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec3 a_vertex;
out vec3 v_vertex;

uniform mat4 rotation;

void main()
{
	v_vertex = (rotation * vec4(a_vertex, 1.0)).xyz;
}

#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 in_point;

void main()
{
	gl_Position = vec4(in_point, 0.0, 1.0);
}

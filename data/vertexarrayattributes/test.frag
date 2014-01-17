#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 fragColor;

in vec4 vertex_color;

void main()
{
	fragColor = vertex_color;
}

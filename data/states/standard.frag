#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 fragColor;

void main()
{
	fragColor = vec4(vec3(1.0), 0.0);
}

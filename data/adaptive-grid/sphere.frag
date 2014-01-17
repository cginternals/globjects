#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 fragColor;

flat in vec3 v_normal;

void main()
{
	vec3 normal = normalize(v_normal);
	fragColor = vec4(v_normal * 0.5 + 0.5, 1.0);
}

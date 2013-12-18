#version 140
#extension GL_ARB_explicit_attrib_location : require

in vec3 normal;
in vec4 vertex_color;

layout (location = 0) out vec4 fragColor;

void main() {
	fragColor = vertex_color;
}

#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 modelmatrix;
uniform mat4 viewprojectionmatrix;
uniform mat3 normalmatrix;

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;

out vec3 normal;

void main() {
	normal = normalmatrix * a_normal;
	gl_Position = viewprojectionmatrix * modelmatrix * vec4(a_vertex, 1.0);
}
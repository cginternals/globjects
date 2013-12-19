#version 430

uniform mat4 modelmatrix;
uniform mat4 viewprojectionmatrix;
uniform mat3 normalmatrix;
uniform vec4 color;

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;

out vec3 normal;
out float z;
out vec4 vertex_color;

void main() {
	normal = normalmatrix * a_normal;
	vec4 pos = viewprojectionmatrix * modelmatrix * vec4(a_vertex, 1.0);
	gl_Position = pos;
	z = pos.z;
	vertex_color = color;
}

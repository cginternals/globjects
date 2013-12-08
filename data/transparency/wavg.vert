#version 430

uniform mat4 modelmatrix;
uniform mat4 viewprojectionmatrix;
uniform mat3 normalmatrix;
uniform vec4 color;

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;

out vec3 normal;
out vec4 vertexColor;

void main() {
	normal = normalmatrix * inNormal;
	gl_Position = viewprojectionmatrix * modelmatrix * vec4(inVertex, 1.0);
	vertexColor = color;
}
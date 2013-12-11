#version 430

uniform mat4 modelmatrix;
uniform mat4 viewprojectionmatrix;

uniform vec4 color;

layout (location = 0) in vec3 inVertex;

out vec4 vertexColor;

void main() {
	gl_Position = viewprojectionmatrix * modelmatrix * vec4(inVertex, 1.0);
	vertexColor = color;
}
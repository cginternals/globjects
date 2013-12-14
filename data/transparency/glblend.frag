#version 140

in vec3 normal;
in vec4 vertex_color;

out vec4 fragColor;

void main() {
	fragColor = vertex_color;
}
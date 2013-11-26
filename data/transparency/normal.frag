#version 140

in vec3 normal;
uniform vec4 color;

out vec4 fragColor;

void main() {
	fragColor = color;
}
#version 430

buffer ABufferEntry {
	int next;
	float depth;
	vec4 color;
}

in vec3 normal;
uniform vec4 color;

out vec4 fragColor;

void main() {
	fragColor = color;
}
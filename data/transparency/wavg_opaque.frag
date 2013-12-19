#version 430

in vec3 normal;
in vec4 vertex_color;

layout(location = 0) out vec4 outColor;

void main() {
	if (vertex_color.a < 0.9999) {
		discard;
	}

	outColor = vertex_color;
}

#version 430

in vec3 normal;
in vec4 vertexColor;

layout(location = 0) out vec4 outColor;

void main() {
	if (vertexColor.a < 0.9999) {
		discard;
	}

	outColor = vertexColor;
}
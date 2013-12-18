#version 430

layout(std430, binding = 0) buffer DepthComplexity {
	uint n[]; // Stores the number of transcluent fragments per pixel
};

uniform ivec2 screenSize;

in vec3 normal;
in vec4 vertex_color;
in ivec4 gl_FragCoord;

layout(location = 1) out vec4 outColor;

void main() {
	if (vertex_color.a > 0.9999) {
		discard;
	}

	atomicAdd(n[gl_FragCoord.y * screenSize.x + gl_FragCoord.x], 1);
	outColor = vec4(vertex_color.rgb * vertex_color.a, vertex_color.a);
}

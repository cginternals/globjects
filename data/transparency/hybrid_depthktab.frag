#version 430

const float MAX_UINT = 1 << 31;
const uint ABUFFER_SIZE = 8;

layout(std430, binding = 0) buffer DepthKTab {
	uint[] depth;
}

uniform ivec2 screenSize;

in float z;
out vec4 vertexColor;
in ivec2 gl_FragCoord;

void main() {
	if (vertexColor.a > 0.9999) {
		discard;
	}
	uint zi = uint(z * MAX_UINT);

	uint baseIndex = gl_FragCoord.y * screenSize.x + gl_FragCoord.x;
	for (int i = 0; i < ABUFFER_SIZE; ++i) {
		zi = max(atomicMin(depth[baseIndex + i], zi), zi);
	}
}
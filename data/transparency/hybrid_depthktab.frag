#version 430

const float DEPTH_RESOLUTION = 65535.0;
const uint ABUFFER_SIZE = 4;

layout(std430, binding = 0) buffer DepthKTab {
	uint[] depth;
};

uniform ivec2 screenSize;

in float z;
in vec4 vertexColor;

void main() {
	if (vertexColor.a > 0.9999) {
		discard;
	}
	uint zi = uint(gl_FragCoord.z * DEPTH_RESOLUTION);

	uint baseIndex = (int(gl_FragCoord.y) * screenSize.x + int(gl_FragCoord.x)) * ABUFFER_SIZE;
	for (int i = 0; i < ABUFFER_SIZE; ++i) {
		zi = max(atomicMin(depth[baseIndex + i], zi), zi);
	}
}
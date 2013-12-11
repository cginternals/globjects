#version 430

const float DEPTH_RESOLUTION = float((1 << 24) - 1);
const float ALPHA_RESOLUTION = float((1 << 8) - 1);
const uint MAX_UINT = 0xFFFFFFFF;
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
	uint za = (uint(gl_FragCoord.z * DEPTH_RESOLUTION) << 8) | uint(vertexColor.a * ALPHA_RESOLUTION); // pack depth (24 bit) & alpha (8 bits) into a uint

	uint baseIndex = (int(gl_FragCoord.y) * screenSize.x + int(gl_FragCoord.x)) * ABUFFER_SIZE;
	for (int i = 0; i < ABUFFER_SIZE; ++i) {
		if (za < MAX_UINT) {
			za = max(atomicMin(depth[baseIndex + i], za), za);
		}
	}
}
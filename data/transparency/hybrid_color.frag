#version 430

layout(early_fragment_tests) in;

const float DEPTH_RESOLUTION = float((1 << 24) - 1);
const float ALPHA_RESOLUTION = float((1 << 8) - 1);
const uint ABUFFER_SIZE = 4;
const uint MAX_UINT = 0xFFFFFFFF;
const uint VISIBILITY_KTAB_SIZE = ABUFFER_SIZE + 1; // one additional value for the wavg tail

layout(std430, binding = 0) buffer DepthKTab {
	uint[] depth;
};

layout(std430, binding = 1) buffer VisibilityKTab {
	float[] visibility;
};

layout(std430, binding = 2) buffer DepthComplexity {
	uint n[];
};

uniform ivec2 screenSize;

in vec3 normal;
in vec4 vertexColor;

layout(location = 0) out vec4 coreColor;
layout(location = 1) out vec4 accColor;

void main() {
	if (vertexColor.a > 0.9999) {
		discard;
	}

	ivec2 screenCoord = ivec2(gl_FragCoord.xy);
	uint baseIndex = screenCoord.y * screenSize.x + screenCoord.x;

	uint packedDepth = uint(gl_FragCoord.z * DEPTH_RESOLUTION);
	uint zi = depth[baseIndex * ABUFFER_SIZE + ABUFFER_SIZE - 1] >> 8;
	if (packedDepth > zi) { // if depth is > than the last value in the k-TAB, the fragment belongs to the tail
		atomicAdd(n[baseIndex], 1);
		accColor = vec4(vertexColor.rgb * vertexColor.a, vertexColor.a);
	} else {
		for (int i = 0; i < ABUFFER_SIZE; ++i) {
			zi = depth[baseIndex * ABUFFER_SIZE + i] >> 8;
			if (abs(packedDepth - zi) < 2) { // depth imprecision issue
				float v = visibility[baseIndex * VISIBILITY_KTAB_SIZE + i];
				coreColor = vec4(vertexColor.rgb * v, 1.0);
				break;
			}
		}
	}
}
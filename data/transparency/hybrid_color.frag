#version 430

const float DEPTH_RESOLUTION = float((1 << 24) - 1);
const float ALPHA_RESOLUTION = float((1 << 8) - 1);
const uint ABUFFER_SIZE = 4;
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
in float z;
in vec4 vertexColor;

layout(location = 0) out vec4 coreColor;
layout(location = 1) out vec4 accColor;

void main() {
	if (vertexColor.a > 0.9999) {
		discard;
	}

	ivec2 screenCoord = ivec2(gl_FragCoord.xy);
	uint packedDepth = uint(gl_FragCoord.z * DEPTH_RESOLUTION);

	uint baseIndex = screenCoord.y * screenSize.x + screenCoord.x;

	uint cache[ABUFFER_SIZE];
	cache[ABUFFER_SIZE - 1] = depth[baseIndex * ABUFFER_SIZE + ABUFFER_SIZE - 1] >> 8;
	if (packedDepth > cache[ABUFFER_SIZE - 1]) {
		atomicAdd(n[baseIndex], 1);
		accColor = vec4(vertexColor.rgb * vertexColor.a, vertexColor.a);
	} else {
		coreColor = vec4(vec3(0.2), 1.0);
	}
}
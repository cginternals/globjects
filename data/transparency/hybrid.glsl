#include </transparency/hybrid_definitions>

const float DEPTH_RESOLUTION = float((1 << 24) - 1);
const float ALPHA_RESOLUTION = float((1 << 8) - 1);
const uint MAX_UINT = 0xFFFFFFFF;

const int VISIBILITY_KTAB_SIZE = ABUFFER_SIZE + 1; // one additional value for the wavg tail

layout(std430, binding = 0) buffer DepthKTab {
	uint[] depthKTab;
};

layout(std430, binding = 1) buffer VisibilityKTab {
	float[] visibilityKTab;
};

layout(std430, binding = 2) buffer DepthComplexity {
	uint[] depthComplexity;
};

uniform ivec2 screenSize;

// pack depth (24 bit) & alpha (8 bits) into a uint
uint pack(float depth, float alpha) {
	return (uint(depth * DEPTH_RESOLUTION) << 8) | uint(alpha * ALPHA_RESOLUTION); 
}

float unpackAlpha(uint packedDepthAlpha) {
	return float(packedDepthAlpha & 0xFF) / ALPHA_RESOLUTION;
}

uint unpackDepthInt(uint packedDepthAlpha) {
	return packedDepthAlpha >> 8;
}

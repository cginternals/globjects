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
uniform sampler2D opaqueBuffer;
uniform sampler2D coreBuffer;
uniform sampler2D accumulationBuffer;

in ivec4 gl_FragCoord;

layout (location = 0) out vec4 fragColor;

vec4 blend(vec4 dst, vec4 src) {
	return dst + src * (1.0 - dst.a);
}

void main() {
	vec4 opaque = texelFetch(opaqueBuffer, gl_FragCoord.xy, 0);
	float z = float(depth[(gl_FragCoord.y * screenSize.x + gl_FragCoord.x) * ABUFFER_SIZE] >> 8) / DEPTH_RESOLUTION;
	float a = visibility[(gl_FragCoord.y * screenSize.x + gl_FragCoord.x) * VISIBILITY_KTAB_SIZE];

	vec4 accumulated = texelFetch(accumulationBuffer, gl_FragCoord.xy, 0);
	uint currentN = n[gl_FragCoord.y * screenSize.x + gl_FragCoord.x];

	vec4 weightedAverage = vec4(0.0);
	if (currentN > 0) {
		weightedAverage = vec4(accumulated.rgb / accumulated.a, accumulated.a / currentN);
	}
	weightedAverage.a = pow(1 - weightedAverage.a, currentN);

	vec4 c = weightedAverage * (1 - weightedAverage.a) + opaque * weightedAverage.a;

	vec4 core = texelFetch(coreBuffer, gl_FragCoord.xy, 0);

	fragColor = c;
}
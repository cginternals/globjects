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

layout (location = 0) out vec4 outColor;

void main() {
	vec4 opaque = texelFetch(opaqueBuffer, gl_FragCoord.xy, 0);
	vec4 core = texelFetch(coreBuffer, gl_FragCoord.xy, 0);
	vec4 accumulated = texelFetch(accumulationBuffer, gl_FragCoord.xy, 0);
	uint currentN = n[gl_FragCoord.y * screenSize.x + gl_FragCoord.x];
	float v = visibility[(gl_FragCoord.y * screenSize.x + gl_FragCoord.x) * VISIBILITY_KTAB_SIZE + ABUFFER_SIZE];

	// compute wavg tail
	vec4 weightedAverage = vec4(0.0, 0.0, 0.0, 1.0);
	if (currentN > 0) {
		weightedAverage = vec4(accumulated.rgb / accumulated.a, accumulated.a / currentN);
		weightedAverage.a = pow(abs(1.0 - weightedAverage.a), currentN);
	}

	// blend all together
	vec3 color = core.rgb 
		+ weightedAverage.rgb * (1.0 - v) * (1.0 - weightedAverage.a)
		+ opaque.rgb * (1.0 - v) * weightedAverage.a;

	outColor = vec4(color, 1.0);
}
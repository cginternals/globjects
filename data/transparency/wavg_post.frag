#version 430

layout(std430, binding = 0) buffer DepthComplexity {
	uint n[];
};

uniform ivec2 screenSize;
uniform sampler2D opaqueBuffer;
uniform sampler2D accumulationBuffer;

in ivec4 gl_FragCoord;

layout(location = 0) out vec4 outColor;

void main() {
	vec4 opaque = texelFetch(opaqueBuffer, gl_FragCoord.xy, 0);
	vec4 accumulated = texelFetch(accumulationBuffer, gl_FragCoord.xy, 0);
	uint currentN = n[gl_FragCoord.y * screenSize.x + gl_FragCoord.x];

	vec4 weightedAverage = vec4(0.0);
	if (currentN > 0) {
		weightedAverage = vec4(accumulated.rgb / accumulated.a, accumulated.a / currentN);
	}
	weightedAverage.a = pow(1 - weightedAverage.a, currentN);

	outColor = weightedAverage * (1 - weightedAverage.a) + opaque * weightedAverage.a;
}

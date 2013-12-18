#version 430
#extension GL_ARB_shading_language_include : require

#include </transparency/hybrid.glsl>

uniform sampler2D opaqueBuffer;
uniform sampler2D coreBuffer;
uniform sampler2D accumulationBuffer;

in ivec4 gl_FragCoord;

layout (location = 0) out vec4 outColor;

void main() {
	vec4 opaque = texelFetch(opaqueBuffer, gl_FragCoord.xy, 0);
	vec4 core = texelFetch(coreBuffer, gl_FragCoord.xy, 0);
	vec4 accumulated = texelFetch(accumulationBuffer, gl_FragCoord.xy, 0);
	uint depthN = depthComplexity[gl_FragCoord.y * screenSize.x + gl_FragCoord.x];
	float visibility = visibilityKTab[(gl_FragCoord.y * screenSize.x + gl_FragCoord.x) * VISIBILITY_KTAB_SIZE + ABUFFER_SIZE];

	// compute wavg tail
	vec4 weightedAverage = vec4(0.0, 0.0, 0.0, 1.0);
	if (depthN > 0) {
		weightedAverage = vec4(accumulated.rgb / accumulated.a, accumulated.a / depthN);
		weightedAverage.a = pow(abs(1.0 - weightedAverage.a), depthN);
	}

	// blend all together
	vec3 color = core.rgb 
		+ weightedAverage.rgb * (1.0 - visibility) * (1.0 - weightedAverage.a)
		+ opaque.rgb * (1.0 - visibility) * weightedAverage.a;

	outColor = vec4(color, 1.0);
}

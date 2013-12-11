#version 430
#extension GL_ARB_shading_language_include : require

layout(early_fragment_tests) in;

#include </transparency/hybrid.glsl>

in vec4 vertexColor;

layout(location = 0) out vec4 coreColor;
layout(location = 1) out vec4 accColor;

void main() {
	if (vertexColor.a > 0.9999) {
		discard;
	}

	ivec2 screenCoord = ivec2(gl_FragCoord.xy);
	uint baseIndex = screenCoord.y * screenSize.x + screenCoord.x;

	uint depthInt = uint(gl_FragCoord.z * DEPTH_RESOLUTION);
	uint cachedDepthInt = unpackDepthInt(depthKTab[baseIndex * ABUFFER_SIZE + ABUFFER_SIZE - 1]);
	if (depthInt > cachedDepthInt) { // if depth is > than the last value in the k-TAB, the fragment belongs to the tail
		atomicAdd(depthComplexity[baseIndex], 1);
		accColor = vec4(vertexColor.rgb * vertexColor.a, vertexColor.a);
	} else {
		for (int i = 0; i < ABUFFER_SIZE; ++i) {
			cachedDepthInt = unpackDepthInt(depthKTab[baseIndex * ABUFFER_SIZE + i]);
			if (abs(depthInt - cachedDepthInt) < 2) { // depth imprecision issue
				float visibility = visibilityKTab[baseIndex * VISIBILITY_KTAB_SIZE + i];
				coreColor = vec4(vertexColor.rgb * visibility, 1.0);
				break;
			}
		}
	}
}
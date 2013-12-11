#version 430
#extension GL_ARB_shading_language_include : require

layout(early_fragment_tests) in;

#include </transparency/hybrid.glsl>

in vec4 vertexColor;

void main() {
	if (vertexColor.a > 0.9999) {
		discard;
	}
	uint packedDepth = pack(gl_FragCoord.z, vertexColor.a);

	uint baseIndex = (int(gl_FragCoord.y) * screenSize.x + int(gl_FragCoord.x)) * ABUFFER_SIZE;
	for (int i = 0; i < ABUFFER_SIZE; ++i) {
		if (packedDepth < MAX_UINT) {
			packedDepth = max(atomicMin(depthKTab[baseIndex + i], packedDepth), packedDepth);
		}
	}
}
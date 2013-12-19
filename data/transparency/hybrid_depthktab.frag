#version 430
#extension GL_ARB_shading_language_include : require

layout(early_fragment_tests) in;

#include </transparency/hybrid.glsl>

in vec4 vertex_color;

void main() {
	if (vertex_color.a > 0.9999) {
		discard;
	}

	// Store z and alpha into 32bit integer
	uint packedDepth = pack(gl_FragCoord.z, vertex_color.a);

	uint baseIndex = (int(gl_FragCoord.y) * screenSize.x + int(gl_FragCoord.x)) * ABUFFER_SIZE;

	// do ascending insert sort (since gl_FragCoord.z occupies the most significant bits the entries will be sorted by depth first)
	for (int i = 0; i < ABUFFER_SIZE; ++i) {
		if (packedDepth < MAX_UINT) {
			// replaces depthKTab[baseIndex + i] with packedDepth if packedDepth < depthKTab[baseIndex + i] 
			packedDepth = max(atomicMin(depthKTab[baseIndex + i], packedDepth), packedDepth);
		}
	}
}

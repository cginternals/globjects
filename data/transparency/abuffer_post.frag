#version 430
#extension GL_ARB_shading_language_include : require

#include </transparency/abuffer.glsl>

uniform vec4 backgroundColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D opaqueBuffer;

layout (pixel_center_integer) in ivec4 gl_FragCoord;

layout (location = 0) out vec4 fragColor;

// blends dst over src
vec4 blend(vec4 dst, vec4 src) {
	return dst + src * (1.0 - dst.a);
}

void main() {
	vec4 opaque = texelFetch(opaqueBuffer, gl_FragCoord.xy, 0);
	vec4 color = vec4(0.0);

	// retrieve head pointer
	int index = headList[gl_FragCoord.y * screenSize.x + gl_FragCoord.x].startIndex;
	if (index >= 0) {
		// copy list elements to contiguous local buffer
		ABufferEntry fragments[ABUFFER_SIZE];
		int numFragments = 0;
		int pos = 0;
		for (int i = 0; i < ABUFFER_SIZE; ++i) {
			if (index >= 0) {
				if (list[index].z < opaque.a) { // ignore fragments behind the nearest opaque fragment (whose z is stored in the a-component)
					fragments[pos++] = list[index];
					++numFragments;
				}
				index = list[index].next;
			} 
		}

		// bubble sort
		for (int i = numFragments - 2; i >= 0; --i) {
			for (int j = 0; j <= i; ++j) {
				if (fragments[j].z > fragments[j + 1].z) {
					ABufferEntry tmp = fragments[j + 1];
					fragments[j + 1] = fragments[j];
					fragments[j] = tmp;
				}
			}
		}

		// blend translucent fragments together from front to back
		for (int i = 0; i < numFragments; ++i) {
			color = blend(color, fragments[i].color);
		}
	}

	// blend opaque fragment (or background)
	fragColor = blend(color, vec4(opaque.rgb, 1.0));
}

#version 430

const uint ABUFFER_SIZE = 32;

struct ABufferEntry {
	vec4 color;
	float z;
	int next;
};

struct Head {
	int startIndex;
	int size;
};

layout (std430, binding = 0) buffer LinkedList {
	ABufferEntry list[];
};

layout (std430, binding = 1) buffer HeadBuffer {
	Head headList[];
};

uniform ivec2 screenSize;
uniform vec4 backgroundColor = vec4(1.0, 1.0, 1.0, 1.0);

layout (pixel_center_integer) in ivec4 gl_FragCoord;
layout (location = 0) out vec4 fragColor;

vec4 blend(vec4 dst, vec4 src) {
	return dst + src * (1.0 - dst.a);
}

void main() {
	// retrieve head pointer
	int index = headList[gl_FragCoord.y * screenSize.x + gl_FragCoord.x].startIndex;
	if (index < 0) {
		discard;
	}

	// copy list elements to contiguous local buffer
	ABufferEntry fragments[ABUFFER_SIZE];
	int numFragments = 0;
	for (int i = 0; i < ABUFFER_SIZE; ++i) {
		if (index >= 0) {
			fragments[i] = list[index];
			index = list[index].next;
			++numFragments;
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

	// blend together
	vec4 color = vec4(0.0);
	for (int i = 0; i < numFragments; ++i) {
		color = blend(color, fragments[i].color);
	}

	// lastly, blend background
	fragColor = blend(color, backgroundColor);
}
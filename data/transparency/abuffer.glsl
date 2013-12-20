#include </transparency/abuffer_definitions>

uniform ivec2 screenSize;

struct ABufferEntry {
	vec4 color;
	float z;
	int next;
};
layout(std430, binding = 0) buffer LinkedList {
	ABufferEntry list[];
};

struct Head {
	int startIndex;
	int size;
};
layout(std430, binding = 1) buffer HeadBuffer {
	Head headList[];
};

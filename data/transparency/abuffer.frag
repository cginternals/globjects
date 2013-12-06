#version 430

const uint ABUFFER_SIZE = 32;

layout(binding = 0) uniform atomic_uint counter;

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

layout(pixel_center_integer) in ivec4 gl_FragCoord;

uniform vec4 color;
uniform ivec2 screenSize;

in vec3 normal;
in float z;

out vec4 fragColor;

void main() {
	int size = atomicAdd(headList[gl_FragCoord.y * screenSize.x + gl_FragCoord.x].size, 1) + 1;
	if (size > ABUFFER_SIZE) {
		discard;
	}

	int index = int(atomicCounterIncrement(counter));
	int previousHead = atomicExchange(headList[gl_FragCoord.y * screenSize.x + gl_FragCoord.x].startIndex, index);

	list[index].next = previousHead;
	list[index].color = vec4(color.rgb * color.a, color.a); // pre-multiplied alpha
	list[index].z = z;

	fragColor = vec4(vec3(z / 20.0), 1.0);
}
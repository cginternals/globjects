#version 430

uniform vec2 screenSize;

layout(binding = 0) uniform atomic_uint counter;

struct ABufferEntry
{
	vec4 color;
	float z;
	uint next;
};

layout (std430, binding = 0) buffer LinkedList
{
	ABufferEntry list[];
};

layout (std430, binding = 1) buffer Head
{
	int headList[];
};

in vec3 normal;
uniform vec4 color;

out vec4 fragColor;

void main() {
	fragColor = color;
}
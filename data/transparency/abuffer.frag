#version 430

uniform ivec2 screenSize;

layout(binding = 0) uniform atomic_uint counter;

struct ABufferEntry
{
	vec4 color;
	float z;
	int next;
};

layout (std430, binding = 0) buffer LinkedList
{
	ABufferEntry list[];
};

layout (std430, binding = 1) buffer Head
{
	int headList[];
};

layout(pixel_center_integer) in ivec4 gl_FragCoord;

in vec3 normal;
in float z;
uniform vec4 color;

out vec4 fragColor;

void main() {
	uint index = atomicCounterIncrement(counter);
	int previousHead = atomicExchange(headList[gl_FragCoord.y * screenSize.x + gl_FragCoord.x], int(index));
	list[index].next = previousHead;
	list[index].color = vec4(color.rgb * color.a, color.a);
	list[index].z = z;

	fragColor = vec4(vec3(z / 20.0), 1.0);
}
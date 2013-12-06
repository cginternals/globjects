#version 430

const uint NUM_FRAGMENTS = 6;

uniform ivec2 screenSize;

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

layout (pixel_center_integer) in ivec4 gl_FragCoord;
layout (location = 0) out vec4 fragColor;

// float lastZ = 0.0;
// int getClosest(uint head) {
// 	float minZ = hugeZ;
// 	uint minIndex = head;
// 	for (int i = 0; i < NUM_FRAGMENTS; ++i) {
// 		float z = list[head].z;
// 		if (z < minZ) {
// 			minZ = z;
// 			minIndex = head;
// 		}
// 		head = list[head].next;
// 	}
// 	if (minZ >= hugeZ) {
// 		return -1;
// 	}
// 	return int(minIndex);
// }

int getNearestFragment(int head, float lastZ)
{
	int entry = head;
	float minZ = list[entry].z;
	int minEntry = entry;
	for (int i = 0; i < NUM_FRAGMENTS; ++i)
	{
		float z = list[entry].z;
		if (z < minZ && z > lastZ) {
			minZ = z;
			minEntry = entry;
		}
		entry = list[entry].next;
		if (entry < 0) {
			break;
		}
	}
	if (minZ <= lastZ) { // not found
		return -1;
	}
	return minEntry;
}

void blend(inout vec4 dst, vec4 src)
{
	dst = dst + src * (1.0 - dst.a);
	//dst.rgb = dst.a * src.rgb + dst.rgb;
	//dst.a = (1.0 - src.a) * dst.a;
}

void main()
{
	vec4 dst = vec4(0.0);

	int head = headList[gl_FragCoord.y * screenSize.x + gl_FragCoord.x];
	if (head < 0)
	{
		discard;
	}

	//dst.rgb = vec3(list[head].z / 20.0);

	// uint entry = head;
	// for (int i = 0; i < NUM_FRAGMENTS; ++i) {
	// 	if (list[entry].next < 0) {
	// 		dst.g = 1.0;
	// 		break;
	// 	}
	// 	entry = list[entry].next;
	// }
	// dst.a = 1.0;

	//  int closestIndex = getNearestFragment(head, 0.0);
	// dst.rgb = list[closestIndex].color.rgb;

	//dst.r = float(closestIndex) / 50000.0;

	//dst.rgb = list[list[list[head].next].next].color.rgb;

	float lastZ = 0.0;
	for (int i = 0; i < NUM_FRAGMENTS; ++i)
	{
		int closestIndex = getNearestFragment(head, lastZ);
		if (closestIndex < 0) {
			break;
		}
		lastZ = list[closestIndex].z;
		vec4 src = list[closestIndex].color;

		blend(dst, src);
	}

	blend(dst, vec4(0.0, 0.0, 0.0, 0.0));
	fragColor = dst;
}
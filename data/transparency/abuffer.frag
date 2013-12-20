#version 430
#extension GL_ARB_shading_language_include : require

#include </transparency/abuffer.glsl>

layout(binding = 0) uniform atomic_uint counter;

layout(pixel_center_integer) in ivec4 gl_FragCoord;

in vec3 normal;
in float z;
in vec4 vertex_color;

layout(location = 0) out vec4 opaqueFragColorZ;

void main() {
	if (vertex_color.a < 0.9999) { // damned floats...

		// Increment size counter for pixel
		int size = atomicAdd(headList[gl_FragCoord.y * screenSize.x + gl_FragCoord.x].size, 1) + 1;
		if (size > ABUFFER_SIZE) {
			discard;
		}

		// Get index of abuffer entry (current value of counter) and increment global counter
		int index = int(atomicCounterIncrement(counter));

		// Read the last abuffer index from the headlist and replace with current index
		int previousHead = atomicExchange(headList[gl_FragCoord.y * screenSize.x + gl_FragCoord.x].startIndex, index);

		list[index].next = previousHead;
		list[index].color = vec4(vertex_color.rgb * vertex_color.a, vertex_color.a); // pre-multiply alpha
		list[index].z = z;

		discard; // translucent fragments go into the a buffer, but not in the color buffer
	}

	opaqueFragColorZ = vec4(vertex_color.rgb, z);
}

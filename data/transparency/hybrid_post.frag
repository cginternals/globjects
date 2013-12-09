#version 430

const float DEPTH_RESOLUTION = 65535.0;
const uint ABUFFER_SIZE = 4;

layout(std430, binding = 0) buffer DepthKTab {
	uint[] depth;
};

uniform ivec2 screenSize;
uniform sampler2D opaqueBuffer;

in ivec4 gl_FragCoord;

layout (location = 0) out vec4 fragColor;

vec4 blend(vec4 dst, vec4 src) {
	return dst + src * (1.0 - dst.a);
}

void main() {
	vec4 opaque = texelFetch(opaqueBuffer, gl_FragCoord.xy, 0);
	float z = float(depth[(gl_FragCoord.y * screenSize.x + gl_FragCoord.x) * ABUFFER_SIZE + ABUFFER_SIZE - 1]) / DEPTH_RESOLUTION;
	fragColor = vec4(z);
}
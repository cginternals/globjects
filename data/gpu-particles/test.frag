#version 330

uniform sampler2D source;

layout (location = 0) out vec4 fragColor;

void main()
{
	vec4 vertex = texelFetch(source, ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y)), 0);

	fragColor = vertex + vec4(0.01, 0.0, 0.0, 0.0);
}
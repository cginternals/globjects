#version 330

in vec2 texCoord;

uniform sampler2D texture;

layout (location = 0) out vec4 outColor;

void main()
{
	outColor = texture2D(texture, texCoord);
}

#version 330

uniform sampler2D texture;

in vec2 v_uv;
layout (location = 0) out vec4 fragColor;

void main()
{
	fragColor = texture2D(texture, v_uv);
}

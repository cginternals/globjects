#version 330

layout (location = 0) out vec4 fragColor;

in vec4 vertex_color;

void main()
{
	fragColor = vertex_color;
}

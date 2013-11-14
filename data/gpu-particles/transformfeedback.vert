#version 330

uniform float delta; // time delta

in vec4 in_position;
out vec4 out_position;

void main()
{
	out_position = vec4(in_position.xyz * 0.1, 1.0);
	gl_Position = out_position;
}

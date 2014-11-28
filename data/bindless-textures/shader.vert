#version 140
#extension GL_ARB_explicit_attrib_location : enable

uniform mat4 projection;

layout (location=0) in vec3 in_position;
layout (location=1) in vec2 in_texCoord;
layout (location=2) in int in_side;

out vec2 texCoord;
flat out int side;

void main()
{
	gl_Position = projection * vec4(in_position, 1.0);
	texCoord = in_texCoord;
	side = in_side;
}

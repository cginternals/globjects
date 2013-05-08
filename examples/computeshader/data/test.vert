#version 420

uniform mat4 modelView;
uniform mat4 projection;

in vec4 color;

in vec3 position;
in vec2 texCoord0;

out vec2 texCoord;

void main()
{
	texCoord = texCoord0;
	gl_Position = projection * modelView * vec4(position, 1.0);
}


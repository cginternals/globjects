#version 330

uniform mat4 modelView;
uniform mat4 projection;

const float PI = 3.1415926535897932384626433832795;
const vec2 origin = vec2(0.5, 0.5);

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_color;

out vec4 color;

float getTheta(in vec2 position)
{
	if (position.x == 0.0)
	{
		return position.y > 0.0 ? PI / 2.0 : (position.y < 0.0 ? -PI / 2.0 : 0.0);
	}
	
	return atan(position.y, position.x);
}

void main()
{
	gl_Position = projection * modelView * in_position;
	color = in_color;
}

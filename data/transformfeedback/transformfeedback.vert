#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform float deltaT;

layout (location = 0) in vec4 in_position;
out vec4 next_position;

const float PI = 3.1415926535897932384626433832795;
const vec2 origin = vec2(0.5, 0.5);

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
	float r = length(in_position.xy - origin);
	float theta = getTheta(in_position.xy - origin);
	
	theta += PI * deltaT / 2.0;
	
	if (true)
	{
		next_position = vec4(
			origin.x + r*cos(theta),
			origin.y + r*sin(theta),
			0.0,
			1.0
		);
	}
	else
	{
		next_position = in_position;
	}
}

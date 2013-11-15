#version 330

uniform float elapsed; // time delta
uniform sampler3D forces;
uniform uint offset;

const float gravity = 1.0;
const float friction = 0.2;

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_velocity;

out vec4 out_position;
out vec4 out_velocity;

void main()
{
	vec3 p = in_position.xyz;
	vec3 v = in_velocity.xyz;

	vec3 force = texture(forces, p * 0.2 + 0.5).xyz;

	float t = elapsed;
	vec3 g = sign(-p) * (p * p); // gravity to center

	vec3 f  = g * gravity + force;
		 f -= v * friction;

	out_position.xyz  += (v * t) + (0.5 * f * t * t);
	out_velocity.xyz += f * t;
}

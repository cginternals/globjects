#version 330

uniform sampler2D vertices;
uniform sampler2D velocities;
uniform sampler3D forces;
uniform float elapsed; // time delta

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 fragVelocity;

const float gravity = 1.0;
const float friction = 0.2;

void moveParticlesInForceField(in vec4 in_position, in vec4 in_velocity, in float elapsed, in sampler3D forces, out vec4 out_position, out vec4 out_velocity)
{
	vec3 p = in_position.xyz;
	vec3 v = in_velocity.xyz;

	vec3 force = texture(forces, p * 0.2 + 0.5).xyz;

	float t = elapsed;
	vec3 g = sign(-p) * (p * p); // gravity to center

	vec3 f  = (g * gravity + force) - (v * friction);

	out_position = vec4(p + (v * t) + (0.5 * f * t * t), 1.0);
	out_velocity = vec4(v + (f * t), 1.0);
}

void main()
{
	vec4 vert = texelFetch(vertices,   ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y)), 0);
	vec4 vel  = texelFetch(velocities, ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y)), 0);

	vec4 vertOut;
	vec4 velOut;
	moveParticlesInForceField(vert, vel, elapsed, forces, vertOut, velOut);

	fragColor    = vertOut; //vert + vec4(0.01, 0.0, 0.0, 0.0);
	fragVelocity = velOut;  //vec4(1.0, 0.0, 1.0, 1.0);
}
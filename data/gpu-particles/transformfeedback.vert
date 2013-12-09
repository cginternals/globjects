#version 330
#extension GL_ARB_shading_language_include : require

uniform float elapsed; // time delta
uniform sampler3D forces;

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_velocity;

out vec4 out_position;
out vec4 out_velocity;

#include </glow/data/gpu-particles/particleMovement.inc>

void main()
{
	moveParticlesInForceField(in_position, in_velocity, elapsed, forces, out_position, out_velocity);
}

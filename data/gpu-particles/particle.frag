#version 330
#extension GL_ARB_shading_language_include : require

uniform sampler2D vertices;
uniform sampler2D velocities;
uniform sampler3D forces;
uniform float elapsed; // time delta

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 fragVelocity;

#include </glow/data/gpu-particles/particleMovement.inc>

void main()
{
	vec4 vert = texelFetch(vertices,   ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y)), 0);
	vec4 vel  = texelFetch(velocities, ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y)), 0);

	vec4 vertOut;
	vec4 velOut;
	moveParticlesInForceField(vert, vel, elapsed, forces, vertOut, velOut);

	fragColor    = vertOut;
	fragVelocity = velOut;
}
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

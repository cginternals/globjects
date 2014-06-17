#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D colorSource;
uniform sampler2D normalSource;
uniform sampler2D worldCoordSource;
uniform sampler2D depthSource;

uniform vec2 screenSize;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

float depthAt(in vec2 coord)
{
	return texture2D(depthSource, coord).x;
}

void calculateAO(in vec2 tc,
				in float threshold,
				in float aoCap,
				in float aoMultiplier,
				in float depth,
				inout float ao,
				inout int pixelsCaculated)
{
	float d = depthAt(tc);

	if (abs(d-depth) < threshold) {
		// map to AO amount
		ao += min(aoCap, max(0.0, depth - d) * aoMultiplier);
		// propagate to sum
		++pixelsCaculated;
	}
}

const vec2 poisson[16] = vec2[16](
	vec2(+0.007937789,	+0.73124397),
	vec2(-0.10177308,	-0.6509396),
	vec2(-0.9906806,	-0.63400936),
	vec2(+0.96427417,	-0.25506377),
	vec2(+0.7163085,	+0.22836149),
	vec2(-0.65210974,	+0.37117887),
	vec2(-0.12714535,	+0.112056136),
	vec2(+0.48898065,	-0.66669613),
	vec2(-0.9744036,	+0.9155904),
	vec2(+0.9274436,	-0.9896486),
	vec2(+0.9782181,	+0.90990245),
	vec2(-0.5583586,	-0.3614012),
	vec2(-0.5021933,	-0.9712455),
	vec2(+0.3091557,	-0.17652994),
	vec2(+0.4665941,	+0.96454906),
	vec2(-0.461774,		+0.9360856)
);

vec3 unsharpmasking(in vec2 texCoord)
{
	vec3 factor = vec3(1.0);

	float ao = 0.0;
	vec2 t = vec2(2.0) / screenSize;
	float threshold = 1.5;

	int pixelsCaculated = 0;
	float aoCap = 0.50;
	float aoMultiplier= 5000.0;
	float depth = depthAt(texCoord);

	int iterations = 6;
	int kernelSize = 16;

	if (depth < 0.999) {
		for (int i = 0; i < iterations; i++) {

			// Iterate over kernel
			for (int j = 0; j < kernelSize; j++) {
				calculateAO(
					texCoord + poisson[j] * t,
					threshold,
					aoCap,
					aoMultiplier,
					depth,
					ao,
					pixelsCaculated);
			}

			t *= 2.0;
			aoMultiplier *= 1.5;
		}

		ao /= float(pixelsCaculated);
	}

	float valScaled = 1.0 - ao;

	factor *= vec3(valScaled);
	
	return factor;
}

void main()
{
    fragColor = vec4(texture(colorSource, v_uv).rgb * unsharpmasking(v_uv), 1.0);
}

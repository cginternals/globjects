#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec4 in_point;
layout (location = 1) in vec2 in_extent;
layout (location = 2) in vec2 in_fullSize;
layout (location = 3) in vec4 in_color;
layout (location = 4) in float in_average;
layout (location = 5) in float in_min;
layout (location = 6) in float in_max;
layout (location = 7) in float in_median;
layout (location = 8) in int in_id;
layout (location = 9) in int in_count;
layout (location = 10) in float in_random;
layout (location = 11) in float in_random2;

out vec4 vertex_color;

void main()
{
	gl_Position = in_point + 0.001 * vec4(
		in_extent.x + in_fullSize.x + in_average + in_median,
		in_extent.y + in_fullSize.y + in_max + in_min,
		float(in_id),
		0.0
	);
	vertex_color = vec4(in_color.r * in_random, float(in_count) / 20.0, in_color.b * in_random2, 1.0);
}

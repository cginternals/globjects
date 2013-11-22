#version 140
#extension GL_ARB_explicit_attrib_location : require

in vec4 in_point; // 0
in vec2 in_extent; // 1
in vec2 in_fullSize; // 2
in vec4 in_color; // 3
in float in_average; // 4
in float in_min; // 5
in float in_max; // 6
in float in_median; // 7
in int in_id; // 8
in int in_count; // 9
in float in_random; // 10
in float in_random2; // 11

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

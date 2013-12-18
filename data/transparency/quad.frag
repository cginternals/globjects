#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D topLeft;
uniform sampler2D topRight;
uniform sampler2D bottomLeft;
uniform sampler2D bottomRight;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
	vec4 tl = texture(topLeft, v_uv * 2.0);
	vec4 tr = texture(topRight, v_uv * 2.0);
	vec4 bl = texture(bottomLeft, v_uv * 2.0);
	vec4 br = texture(bottomRight, v_uv * 2.0);

	if (v_uv.y > 0.5) {
		if (v_uv.x < 0.5) {
			fragColor = tl;
		} else {
			fragColor = tr;
		}
	} else {
		if (v_uv.x < 0.5) {
			fragColor = bl;
		} else {
			fragColor = br;
		}
	}
}

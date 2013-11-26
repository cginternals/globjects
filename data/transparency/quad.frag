#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D one;
uniform sampler2D two;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
	if (v_uv.x >= 0.5 && v_uv.x <= 0.501) {
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else if (v_uv.x < 0.5) {
		fragColor = texture(one, vec2(v_uv.x * 2.0, v_uv.y));
	} else {
		fragColor = texture(two, vec2((v_uv.x - 0.5) * 2.0, v_uv.y));
	}
	
}

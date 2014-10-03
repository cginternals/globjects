#version 140
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_shading_language_include : require

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
	vec4 color = vec4(0.6, 0.3, 0.3, 1.0);
	
	#include </color.glsl>
	
	fragColor = color;
}

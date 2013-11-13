#version 330
#extension GL_ARB_shading_language_include : require

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
	vec4 color = vec4(0.2, 0.3, 0.4, 1.0);
	
	#include </shaderincludestest/colorReset>
	
	fragColor = color;
}

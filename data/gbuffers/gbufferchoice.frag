#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform int choice;
uniform sampler2D postprocessedSource;
uniform sampler2D colorSource;
uniform sampler2D normalSource;
uniform sampler2D worldCoordSource;
uniform sampler2D depthSource;

uniform float nearZ;
uniform float farZ;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
	switch (choice)
	{
		case 1:
			fragColor = texture(colorSource, v_uv);
		break;
		case 2:
			fragColor = vec4((texture(normalSource, v_uv).rgb + 1.0) * 0.5, 1.0);
		break;
		case 3:
			fragColor = vec4(texture(worldCoordSource, v_uv).rgb * 0.5 + 0.5, 1.0);
		break;
		case 4:
			float depth = texture(depthSource, v_uv).r;
			
			float d = (nearZ*farZ/4.0) / (farZ-depth*(farZ-nearZ));
			
			fragColor = vec4(vec3(d), 1.0);
		break;
		default:
			fragColor = texture(postprocessedSource, v_uv);
		break;
	}
}

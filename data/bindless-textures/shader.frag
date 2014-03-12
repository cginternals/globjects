#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require // for uint64_t

uniform uint64_t textures[4];

layout (location=0) out vec4 outColor;

in vec2 texCoord;
flat in int side;

const vec4 foo[4] = vec4[4](
	vec4(1.0, 0.0, 0.0, 1.0),
	vec4(0.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0)
);

void main()
{
	//~ outColor = foo[side];
	
	sampler2D s = sampler2D(textures[side]);
	
	outColor = texture(s, texCoord);
}

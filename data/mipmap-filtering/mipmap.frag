#version 330

uniform sampler2D source;

in vec2 v_uv;
layout (location = 0) out vec4 fragColor;

const vec3 m[7] = { 
    vec3(1.0, 0.0, 0.0)
  , vec3(1.0, 1.0, 0.0) 
  , vec3(0.0, 1.0, 0.0) 
  , vec3(0.0, 1.0, 1.0) 
  , vec3(0.0, 0.0, 1.0) 
  , vec3(1.0, 0.0, 1.0) 
  , vec3(0.0, 1.0, 1.0)};

void main()
{
	ivec2 s = textureSize(source, 0);

	vec2 d = fwidth(v_uv * s);
	// that is
	/*	
	vec2 dx = dFdx(v_uv * s);
	vec2 dy = dFdy(v_uv * s);
	vec2 d = sqrt(dx * dx + dy * dy);
	*/
	float level = max(0.0, log2(max(d.x, d.y)));
	int i0 = int(floor(level)) % 7;
	int i1 = int(ceil(level)) % 7;

	vec3 o = mix(m[i0], m[i1], level - floor(level));
	
	vec3 me = textureLod(source, v_uv, level).xyz;
	vec3 gt = texture(source, v_uv).xyz; // ground truth 
	vec3 no = textureLod(source, v_uv, 0.0).xyz; // no filtering 
	vec3 er = 100 * abs(me - gt);
	
	// from left to right:
	// mipmap level (colored), our level impl, no mipmap, gpu level impl, error x 100

	fragColor = vec4(mix(o * me, mix(me, mix(no, mix(gt, er * gt,  
					smoothstep( 0.9, 1.1, v_uv.x)),
				smoothstep(+0.23, 0.43, v_uv.x)),
			smoothstep(-0.43, -0.23, v_uv.x)),
		smoothstep(-1.1, -0.9, v_uv.x)), 1.0);
}

#version 330

uniform sampler2D normal;
uniform sampler2D geom;
uniform float timef;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;
in vec3 v_eye;

const vec3 lpos = vec3(0.0, 2.0, 2.0);

const vec3  specular = vec3(1.0, 1.0, 1.0) * 0.1;
const float shininess = 128.0;
const vec3  ambient = vec3(0.08, 0.10, 0.14);
const float ambifake = 0.33;

void main()
{
	vec3 n = normalize(texture2D(normal, v_uv).rgb);
	vec3 g = texture2D(geom, v_uv).rgb;
	
	vec3 l = normalize(lpos - g);
	vec3 e = normalize(v_eye);
	vec3 h = normalize(l + e);

	float ldotn = mix(ambifake, 1.0, dot(l, n));
	float hdotn = dot(h, n);

	if(ldotn > 0.0)
		fragColor = vec4(
		  specular * clamp(pow(hdotn, shininess), 0.0, 1.0) 
		+ mix(ambient, vec3(1.0), ldotn), 1.0);
	else
		fragColor = vec4(ambient, 1.0);
}
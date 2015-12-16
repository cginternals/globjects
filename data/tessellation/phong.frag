#version 140

vec3 phong(
	in vec3 eye
,	in vec3 vertex
,	in vec3 normal
,	in vec3 light
,   in vec3 color
, 	in vec3 specular
,	in float shininess 
,	in vec3 ambient
,	in float ambifake)
{	
	vec3 l = normalize(light - vertex);
	vec3 e = normalize(eye);
	vec3 h = normalize(l + e);
	
	float ldotn = mix(ambifake, 1.0, dot(l, normal));
	float hdotn = dot(h, normal);

	vec3 s = specular * clamp(pow(hdotn, shininess), 0.0, 1.0);
	vec3 d = mix(ambient, color, max(0.0, ldotn));

	return s + d;
}

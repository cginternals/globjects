#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform float timef;

vec3 phong(
	in vec3 eye
,	in vec3 vertex
,	in vec3 normal
,	in vec3 light
,   in vec3 color
, 	in vec3 specular
,	in float shininess 
,	in vec3 ambient
,	in float ambifake);

layout (location = 0) out vec4 fragColor;

in vec3 g_vertex;
in vec3 g_tridist;

void main()
{
	vec3 vertex = (g_vertex);
	float edgedist = min(g_tridist.x, min(g_tridist.y, g_tridist.z));
	float edge = 1.0 - smoothstep(0.0, 0.04, edgedist);
	
	fragColor = vec4(phong( 
	  vertex	
	, vertex
	, normalize(vertex)
	, vec3(0.0, 2.0, 2.0) // light pos
	, vec3(1.0) // color
	, vec3(0.1) // specular
	, 32.0 // shininess
	, vec3(0.08, 0.10, 0.14) // ambient
	, 0.33 // ambifake
	) - vec3(edge) * 0.08, 1.0);
}

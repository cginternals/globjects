#version 330

layout (location = 0) out vec4 normalOut;
layout (location = 1) out vec4 geomOut;

smooth in vec3 v_normal;
smooth in vec3 v_vertex;

void main()
{
	vec3 normal = normalize(v_normal);
	normalOut = vec4(normal, 1.0);
	geomOut = vec4(v_vertex, 1.0);
}

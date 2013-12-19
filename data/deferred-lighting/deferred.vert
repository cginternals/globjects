#version 330

uniform mat4 modelView;
uniform mat4 projection;

layout (location = 0) in vec3 a_vertex;
out vec2 v_uv;

void main()
{
	v_uv = a_vertex.xy;
	gl_Position = projection * modelView * vec4(a_vertex, 1.0);
}

#version 330

uniform mat4 transformi;

layout (location = 0) in vec2 a_vertex;
out vec2 v_uv;
out vec3 v_eye;

void main()
{
	v_uv = a_vertex * 0.5 + 0.5;
	v_eye = (transformi * vec4(a_vertex, 0.0, 1.0)).xyz;
	gl_Position = vec4(a_vertex, 0.0, 1.0);
}

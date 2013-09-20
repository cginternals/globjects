#version 330

uniform mat4 modelView;
uniform mat4 projection;

in vec4 a_vertex;
flat out float v_type;

void main()
{
	v_type = a_vertex.w;
	gl_Position = projection * modelView * vec4(a_vertex.xyz, 1.0);
}

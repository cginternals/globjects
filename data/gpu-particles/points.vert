#version 330

uniform mat4 viewProjection;
uniform sampler2D vertices;

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec4 a_velocity;

out float v_scale;
out vec4 v_color;

void main()
{
	int y = gl_VertexID / 1000;
	int x = gl_VertexID % 1000;
	vec4 vertex = texelFetch(vertices, ivec2(x, y), 0);
	vec4 velocity = a_velocity;

	v_scale = 0.006;
	v_color = vec4(normalize(velocity.xyz) * 0.5 + 0.5, 0.02);
	v_color = vec4(1.0, 0.0, 1.0, 1.0);
	gl_Position = viewProjection * vertex;
}

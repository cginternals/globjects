#version 330

uniform mat4 modelViewProjection;
uniform mat4 R;
uniform float S;

layout (location = 0) in vec3 a_vertex;
out vec2 v_uv;

void main()
{
	v_uv = a_vertex.xy * S;
	gl_Position = modelViewProjection * R* vec4(a_vertex.xzy * 16.0, 1.0);
}

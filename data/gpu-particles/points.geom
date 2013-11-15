#version 430

uniform mat4 viewProjection;
uniform float aspect;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float v_scale[];

out vec2 g_uv;

void main()
{
	vec4 p = viewProjection * gl_in[0].gl_Position;

	float k = v_scale[0];
	vec4 u = vec4(1.0, 0.0, 0.0, 0.0);	
	vec4 v = vec4(0.0, aspect, 0.0, 0.0);
	
	g_uv = vec2(-1.0, -1.0);
	gl_Position = p - normalize(-u - v) * k;
	EmitVertex();

	g_uv = vec2(-1.0,  1.0);
	gl_Position = p - normalize(-u + v) * k;
	EmitVertex();
	
	g_uv = vec2( 1.0, -1.0);
	gl_Position = p - normalize(+u - v) * k;
	EmitVertex();

	g_uv = vec2( 1.0,  1.0);
	gl_Position = p - normalize(+u + v) * k;
	EmitVertex();

	EndPrimitive();	
}

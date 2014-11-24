#version 150

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 te_vertex[3];
in vec3 te_tesscs[3];

out vec3 g_vertex;
out vec3 g_tridist;

void main()
{
	g_tridist = vec3(1.0, 0.0, 0.0);
	g_vertex = te_vertex[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	g_tridist = vec3(0.0, 1.0, 0.0);
	g_vertex = te_vertex[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	g_tridist = vec3(0.0, 0.0, 1.0);	
	g_vertex = te_vertex[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}

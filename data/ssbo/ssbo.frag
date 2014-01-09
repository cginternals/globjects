#version 430

layout (location = 0) out vec4 fragColor;

layout (std430, binding = 1) buffer MyBuffer
{
	int data[];
};

in vec2 v_uv;

uniform int maximum;
uniform int rowCount;
uniform int columnCount;

void main()
{
	int i = int(v_uv.x*rowCount);
	int j = int(v_uv.y*columnCount);
	int index = j*rowCount+i;
	float f = float(data[index])/float(maximum);
	fragColor = vec4(f,0.0,0.0,1.0);
}

#version 430

layout (location = 0) out vec4 outColor;

layout (std430, binding = 1) buffer MyBuffer
{
	int data[];
};

in vec2 texCoord;

uniform int maximum;
uniform int rowCount;
uniform int columnCount;

void main()
{
	int i = int(texCoord.x*rowCount);
	int j = int(texCoord.y*columnCount);
	int index = j*rowCount+i;
	float f = float(data[index])/float(maximum);
	outColor = vec4(f,0.0,0.0,1.0);
}

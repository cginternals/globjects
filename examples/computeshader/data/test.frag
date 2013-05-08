#version 420

uniform sampler2D texture;

in vec2 texCoord;

out vec4 outColor;

void main()
{
	vec4 col = texture2D(texture, texCoord);
	outColor = vec4(col.xy, 1.0, 1.0);
	//outColor = vec4(0.0,0.0,1.0,1.0);
}

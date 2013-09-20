#version 330

flat in float v_type;

layout (location = 0) out vec4 fragColor;

void main()
{
	float t = 1.f - v_type * 0.5;

	float z = gl_FragCoord.z; 

	float znear = 0.1;
	float zfar  = 8.0;

	// complete function
	// z = (2.0 * zfar * znear / (zfar + znear - (zfar - znear) * (2.0 * z - 1.0)));
	// normalized to [0,1]
	// z = (z - znear) / (zfar - znear);

	// simplyfied with wolfram alpha
	z = - znear * z / (zfar * z - zfar - znear * z);

	fragColor = vec4(vec3(mix(t, 1.0, z * z)), 1.0);
}

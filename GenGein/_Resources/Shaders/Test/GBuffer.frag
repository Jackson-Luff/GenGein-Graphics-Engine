// GBuffer Fragment Buffer

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vCoord;

vec3 diffuse = vec3(1.0, 0.0, 0.9);

void main()
{
	gPosition = vPosition;
	gNormal   = vNormal;
	gAlbedoSpec	= vec4(vCoord, 1.0, 1.0);
}
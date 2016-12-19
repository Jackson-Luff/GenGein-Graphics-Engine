// GBuffer Fragment Buffer

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vCoord;

vec3 diffuse = vec3(0.28, 0.28, 0.28);

uniform sampler2D tConcrete;

void main()
{
	vec3 conNorm  = vPosition.xyz;//texture(tConcrete, vCoord).rgb;
	
	gPosition = vec4(vPosition, 1.0);
	gNormal   = vec4(vNormal, 1.0);
	gAlbedoSpec	= vec4(diffuse, 1.0);
}
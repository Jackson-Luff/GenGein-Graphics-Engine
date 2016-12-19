in vec3 vPosition;

uniform samplerCube SkyBox;

layout(location = 2) out vec4 gAlbedoSpec;

void main()
{
	gAlbedoSpec = texture(SkyBox, vPosition);
}
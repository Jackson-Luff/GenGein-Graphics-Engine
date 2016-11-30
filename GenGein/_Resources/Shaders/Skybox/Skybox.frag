in vec3 vPosition;

uniform samplerCube SkyBox;

out vec4 pixelColour;

void main()
{
	pixelColour = texture(SkyBox, vPosition);
}
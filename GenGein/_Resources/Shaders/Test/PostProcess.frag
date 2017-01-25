// GBuffer Fragment Buffer

in vec2 vCoord;
uniform sampler2D finalImage;

out vec4 FragmentColour;
void main()
{
	FragmentColour = texture(finalImage, vCoord);
}

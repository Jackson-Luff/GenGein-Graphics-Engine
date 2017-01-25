// GBuffer Fragment Buffer

in vec3 vPosition;

uniform mat4 LightMatrix;
void main()
{
	glm::mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);
	glm::mat4 depthBiasMVP = biasMatrix*depthMVP;

	gl_Position = LightMatrix * vPosition;
}
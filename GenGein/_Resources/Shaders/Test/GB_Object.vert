// GBuffer Vertex Shader

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertCoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vCoord;

layout(std140) uniform CamUB
{
	mat4 camWrld;
	mat4 camProj;
	mat4 camView;
};

uniform mat4 LocalMatrix;

void main()
{
	// Generate world-space vertex position
	vec3 LSNormal 	= mat3(LocalMatrix) * vertNormal.xyz;
	vec3 LSPosition = vec3(LocalMatrix * vec4(vertPos, 1.0));
	
	mat4 camProjView = (camProj * camView );
	gl_Position = camProjView * LocalMatrix * vec4(vertPos, 1.0);
	
	// pass GBuffer relevant information
	vPosition = LSPosition;
	vNormal = normalize(LSNormal);
	vCoord = vertCoord;
}
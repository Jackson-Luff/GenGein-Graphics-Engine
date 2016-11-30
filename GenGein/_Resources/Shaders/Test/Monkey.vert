// Monkey Vertex Shader

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertCoord;

uniform mat4 LocalMatrix;

layout(std140) uniform CamUB
{
	mat4 camWrld;
	mat4 camProj;
	mat4 camView;
};

out vec3 vNormal;
out vec3 vLightDir;

vec3 lightPos = vec3(5.0, 5.0, 0.0);

void main()
{
	// Generate vertex position
	mat4 camera = (camProj * camView );
	vec4 p = camera * (LocalMatrix * vertPos);
	gl_Position = p;
	
	// Diffuse lighting
	vNormal = mat3(LocalMatrix) * vertNormal.xyz;
	vLightDir = normalize(lightPos - vertPos.xyz);
}
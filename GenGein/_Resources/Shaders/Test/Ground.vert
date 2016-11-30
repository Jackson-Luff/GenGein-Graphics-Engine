// Ground Vertex Shader

layout(location=0) in vec4 vertPosition;
layout(location=1) in vec4 vertNormal;
layout(location=2) in vec2 vertUV;

uniform mat4 LocalMatrix;

layout(std140) uniform CamUB
{
	mat4 camWlrd;
	mat4 camProj;
	mat4 camView;
};


out vec3 vPos;
out vec3 vNormal;


void main()
{
	mat4 camera = (camProj * camView);
	vec4 p = camera * (LocalMatrix * vertPosition);
	gl_Position = p;
	
	// Diffuse lighting
	vPos = vertPosition.xyz;
	vNormal = mat3(LocalMatrix) * vertNormal.xyz;
}
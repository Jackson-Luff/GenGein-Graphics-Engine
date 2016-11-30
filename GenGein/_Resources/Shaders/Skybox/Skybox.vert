
layout(std140) uniform CamUB
{
	mat4 camWrld;
	mat4 camProj;
	mat4 camView;
};

layout(location=0) in vec3 vertPosition;
layout(location=1) in vec3 vertNormal;
layout(location=2) in vec3 vertCoord;

out vec3 vPosition;

void main()
{
	mat4 camera = camProj * mat4(mat3(camView));
	vec4 p = camera * vec4(vertPosition,1.0);
	gl_Position = p;
	vPosition = vec3(vertPosition);
}

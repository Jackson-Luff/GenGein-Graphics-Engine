
layout(std140) uniform CameraInfo
{
	vec4 camPosition;
	mat4 camProj;
	mat4 camView;
};

layout(location=0) in vec3 vertPosition;

out vec3 vPosition;

void main()
{
	mat4 camera = camProj * mat4(mat3(camView));
	vec4 p = camera * vec4(vertPosition,1.0);
	gl_Position = p;
	vPosition = vec3(vertPosition);
}

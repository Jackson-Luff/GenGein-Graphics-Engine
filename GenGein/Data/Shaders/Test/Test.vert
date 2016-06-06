#version 440

layout(std140) uniform UniBuff
{
	vec4 camPosition;
	mat4 camProjView;
};

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertNormal;
in layout(location=2) vec2 vertUV;

out vec4 vPosition;

void main()
{
	vec4 position = camProjView * vertPosition;

	gl_Position = position;
	vPosition = position;
}
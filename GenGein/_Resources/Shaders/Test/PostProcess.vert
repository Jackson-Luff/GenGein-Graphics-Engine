// Ground Vertex Shader

layout(location=0) in vec3 vertPosition;
layout(location=1) in vec3 vertNormal;
layout(location=2) in vec2 vertCoord;

out vec2 vCoord;

void main()
{
	gl_Position = vec4(vertPosition, 1.0);
	vCoord = vertCoord;
}

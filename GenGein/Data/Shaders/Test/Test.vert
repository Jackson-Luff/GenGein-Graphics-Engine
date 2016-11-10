layout(std140) uniform UniBuff
{
	vec4 camPosition;
	mat4 camProj;
	mat4 camView;
};

in layout(location=0) vec4 vertPosition;
in layout(location=1) vec4 vertNormal;
in layout(location=2) vec2 vertUV;

out vec3 vNormals;
out vec2 vCoords;

out vec3 vLightDir;
out vec3 vCamPos;

vec3 lightPos = vec3(-20.0, -20.0, 0.0);

void main()
{
	mat4 camera = (camProj * camView);
	vec4 p = camera * vertPosition;
	gl_Position = p;
	
	vNormals = vertNormal.xyz;
	vCoords = vertUV;
	
	// Diffuse lighting
	vLightDir = normalize(lightPos - vertPosition.xyz);
	vCamPos = camPosition.xyz;
}
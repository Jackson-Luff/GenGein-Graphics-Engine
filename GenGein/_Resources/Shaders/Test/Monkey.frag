
in vec3 vNormals;
in vec2 vCoords;

in vec3 vLightDir;
in vec3 vCamPos;

float shininess = 4.0;
vec3 diffuse = vec3(1.0, 0.98, 0.9);
vec3 specular = vec3(0.5, 0.42, 0.32);

uniform sampler2D textureTest;

out vec4 pixelColour;

float sdSphere( vec3 p, float s )
{
  return length(p)-s;
}


void main()
{
	vec3 colour = vec3(1);
	
	vec3 N = normalize(vNormals);
	vec3 L = normalize(vLightDir);
    
	float lambertTerm = clamp(dot(N,L), 0.0, 1.0);
	
	if(lambertTerm >= 0.0)
	{
		vec3 cP = normalize(vCamPos);
		vec3 R  = reflect(-L, N); 
		float s = pow(	max(dot(R,cP), 0.0), shininess);
		
		colour += diffuse * lambertTerm;
		
	}
	
    pixelColour = vec4(vec3(lambertTerm), 1.0);
}
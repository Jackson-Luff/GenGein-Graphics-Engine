// Monkey Fragment Buffer

in vec3 vNormal;
in vec3 vLightDir;

float shininess = 4.0;
vec3 diffuse = vec3(1.0, 0.98, 0.9);
vec3 specular = vec3(0.5, 0.42, 0.32);

out vec4 pixelColour;

void main()
{
	vec3 colour = vec3(0);
	
	vec3 N = normalize(vNormal);
	vec3 L = normalize(vLightDir);
    
	float lambertTerm = clamp(dot(N,L), 0.0, 1.0);
	
	if(lambertTerm >= 0.0)
	{	
		colour += diffuse * lambertTerm;
	}

	pixelColour = vec4(colour, 1.0);
}
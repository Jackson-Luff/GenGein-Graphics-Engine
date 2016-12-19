// PostQuad Fragment Shader

out vec4 pixelColour;
in vec2 vCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

layout(std140) uniform CamUB
{
	mat4 camWrld;
	mat4 camProj;
	mat4 camView;
};

struct Light {
    vec3 Position;
    vec3 Colour;
    
    float Linear;
    float Quadratic;
};

void main()
{
	// Retrieve data from gbuffer
	vec3 camPos = camWrld[3].xyz;
    vec3 FragPos  = texture(gPosition, 	 vCoord).rgb;
    vec3 Normal   = texture(gNormal,  	 vCoord).rgb;
    vec3 Diffuse  = texture(gAlbedoSpec, vCoord).rgb;
	
    vec3 finalColour = Diffuse * 0.9;
	
	Light light;
	light.Position = vec3(5);
	light.Colour = vec3(1.0, 0.98, 0.9);
	
	vec3 lightDir = (light.Position - FragPos);
	vec3 N = normalize(Normal);      
	vec3 L = normalize(lightDir);      

	float lambertTerm = dot(N, L);
	
	if(lambertTerm > 0.0)
	{
		finalColour += light.Colour * Diffuse * lambertTerm;
		
		vec3 E = normalize(camPos);
		vec3 R = reflect(-L, N);
		float s = pow( max(dot(E, R), 0.0), 4.0);
		vec3 specularColour = vec3(0.5, 0.42, 0.32);
		finalColour += specularColour * s;
	}
	
    pixelColour = vec4(Normal, 1.0);
}
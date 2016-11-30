// PostQuad Fragment Shader

out vec4 pixelColour;
in vec2 vCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


void main()
{
	vec3 gPosi = texture(gPosition,  vCoord).rgb;
	vec3 gNorm = texture(gNormal,  	 vCoord).rgb;
	vec3 gAlbe = texture(gAlbedoSpec,vCoord).rgb;

	pixelColour = vec4(gNorm, 1.0);
}
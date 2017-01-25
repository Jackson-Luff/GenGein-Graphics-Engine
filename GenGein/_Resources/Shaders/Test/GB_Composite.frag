// PostQuad Fragment Shader

out vec4 pixelColour;
layout(location = 3) out vec4 gComposite;

in vec2 vCoord;

struct Light {
    vec3 Position;
    vec3 Colour;

    float Linear;
    float Quadratic;
};

layout(std140) uniform CamUB
{
	mat4 camWrld;
	mat4 camProj;
	mat4 camView;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

vec4 FXAA_Dirty(in sampler2D tex, in vec2 fCoord)
{
	//
	float FXAA_SPAN_MAX = 8.0;
	float FXAA_REDUCE_MIN = (1.0 / 8.0);
	float FXAA_REDUCE_MUL = (1.0 / 128.0);

	//
	mediump vec2 inverseVP = vec2(1.0/1600.0, 1.0/900.0);

	//
	vec3 luma = vec3(0.299, 0.587, 0.114);

	vec3 rgbTL = texture2D(tex, fCoord + (vec2(-1.0, -1.0) * inverseVP )).xyz;
	vec3 rgbTR = texture2D(tex, fCoord + (vec2(+1.0, -1.0) * inverseVP )).xyz;
	vec3 rgbBL = texture2D(tex, fCoord + (vec2(-1.0, +1.0) * inverseVP )).xyz;
	vec3 rgbBR = texture2D(tex, fCoord + (vec2(+1.0, +1.0) * inverseVP )).xyz;
	vec4 rgbM  = texture2D(tex, fCoord);

	float lumaTL = dot(luma, rgbTL );
	float lumaTR = dot(luma, rgbTR );
	float lumaBL = dot(luma, rgbBL );
	float lumaBR = dot(luma, rgbBR );
	float lumaM =  dot(luma, rgbM.xyz );

	//
	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));

	//
 	mediump vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = +((lumaTL + lumaTL) - (lumaTR + lumaBR));

	//
	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (FXAA_REDUCE_MUL * 0.25), FXAA_REDUCE_MIN);

	//
	float invDirAdj = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	//
	dir = min(vec2(FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX), dir * invDirAdj)) * inverseVP;

	//
	vec3 rgbA = 0.5 * (
		texture2D(tex, fCoord + (dir * vec2(1.0/3.0 - 0.5))).xyz +
		texture2D(tex, fCoord + (dir * vec2(2.0/3.0 - 0.5))).xyz);

	//
	vec3 rgbB = rgbA * 0.5 + 0.25 * (
		texture2D(tex, fCoord + (dir * vec2(0.0/3.0 - 0.5))).xyz +
		texture2D(tex, fCoord + (dir * vec2(3.0/3.0 - 0.5))).xyz);

	//
	float lumaB = dot(rgbB, luma);

	//
	if((lumaB < lumaMin) || (lumaB > lumaMax))
		return vec4(rgbA, rgbM.a);
	else
		return vec4(rgbB, rgbM.a);
}

void main()
{
	//
	vec3 compColour = vec3(0);

	//
	vec3 camPos = camWrld[3].xyz;
	vec2 winResRatio = vec2(1.0/1600.0, 1.0/900.0);

	//
	vec3 FragPos  = texture(gPosition, 		vCoord).xyz;
  vec3 Normal   = texture(gNormal, 		  vCoord).xyz;
  vec3 Diffuse  = texture(gAlbedoSpec, 	vCoord).xyz;//FXAA_Dirty(gAlbedoSpec, vCoord);

	//
	Light light;
	light.Position = vec3(10);
	light.Colour = vec3(1.0, 0.98, 0.9);

	//
	vec3 lightDir = (light.Position - FragPos);
	vec3 N = normalize(Normal);
	vec3 L = normalize(lightDir);

	float lambertTerm = max(dot(N, L), 0);

	if(lambertTerm > 0.0)
	{
		compColour.rgb += light.Colour * Diffuse.rgb * lambertTerm;

		//vec3 E = normalize(camPos);
		//vec3 R = reflect(-L, N);
		//float s = pow( max(dot(E, R), 0.0), 4.0);
		//vec3 specularColour = vec3(0.5, 0.42, 0.32);
		//Diffuse += specularColour * s;
	}

	gComposite  = vec4(Diffuse, 1.0);
  pixelColour = vec4(Diffuse, 1.0);
}

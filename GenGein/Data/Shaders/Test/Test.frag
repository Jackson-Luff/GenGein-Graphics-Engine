#version 440

in vec4 vPosition;
out vec4 FragColor;

void main()
{
	vec3 colour = vec3(0.5,0.2, 1);
    FragColor = vec4(colour, 1.0);
}
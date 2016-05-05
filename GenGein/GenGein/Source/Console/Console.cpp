#include <iostream>
#include <glm\glm.hpp>

#include "Console.h"

// Print Primitive Types
void Console::PrintText(const char* a_textToPrint)
{	
	printf("%s\n",a_textToPrint);
	return;
}

void Console::PrintFloat(float a_floatToPrint, const char* a_desc)
{	
	printf("%s  %f\n", a_desc, a_floatToPrint);
	return;
}	

void Console::PrintInt(int a_intToPrint, const char* a_desc)
{
	printf("%s  %u\n", a_desc, a_intToPrint);
	return;
}

// Print Vectors
void Console::PrintVector2(glm::vec2 a_vec2ToPrint, const char* a_desc)
{
	printf("%s  X: %f, Y: %f\n", a_desc, a_vec2ToPrint.x, a_vec2ToPrint.y);
	return;
}

void Console::PrintVector3(glm::vec3 a_vec3ToPrint, const char* a_desc)
{
	printf("%s  X: %f, Y: %f, Z: %f\n", a_desc, a_vec3ToPrint.x, a_vec3ToPrint.y, a_vec3ToPrint.z);
	return;
}

void Console::PrintVector4(glm::vec4 a_vec4ToPrint, const char* a_desc)
{
	printf("%s  X: %f, Y: %f, Z: %f, W: %f\n", a_desc, a_vec4ToPrint.x, a_vec4ToPrint.y, a_vec4ToPrint.z, a_vec4ToPrint.w);
	return;
}

// Print Matrices
void Console::PrintMatrix3x3(glm::mat3 a_mat3ToPrint, const char* a_desc)
{
	//#NOTE: slightly inefficient as printf is called 12 times
	for (unsigned int r = 0; r < 3; r++)
	{
		for (unsigned int c = 0; c < 3; c++)
		{
			printf("M(%u: %f", (unsigned int)(r + c), a_mat3ToPrint[r][c]);
		}
		printf("\n");
	}
	return;
}

void Console::PrintMatrix4x4(glm::mat4 a_mat4ToPrint, const char* a_desc)
{
	//#NOTE: slightly inefficient as printf is called 20 times
	for (unsigned int r = 0; r < 4; r++)
	{
		for (unsigned int c = 0; c < 4; c++)
		{
			printf("M(%u): %f", (unsigned int)(r + c), a_mat4ToPrint[r][c]);
		}
		printf("\n");
	}
	return;
}
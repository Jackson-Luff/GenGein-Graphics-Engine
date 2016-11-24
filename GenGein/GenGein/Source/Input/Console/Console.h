#pragma once
#include <glm\fwd.hpp>
 


class Console
{
public:

	enum LOG_TYPE
	{						   // Code is as follows [ignore 0x]: 
		LOG_SUCCESS = 0x0A,	   // 0 = Black    4 = Red       8 = Gray		   C = Light Red
		LOG_WARNING = 0x0E,	   // 1 = Blue     5 = Purple    9 = Light Blue	   D = Light Purple
		LOG_ERROR	= 0x0C,	   // 2 = Green    6 = Yellow    A = Light Green   E = Light Yellow
		LOG_DEFAULT = 0x07,	   // 3 = Aqua     7 = White     B = Light Aqua	   F = Bright White
	};

	// Print Primitive Types
	static void PrintText(const char*);
	static void PrintFloat(float, const char* = "");
	static void PrintInt(int, const char* = "");

	// Print Vectors
	static void PrintVector2(glm::vec2&, const char* = "");
	static void PrintVector3(glm::vec3&, const char* = "");
	static void PrintVector4(glm::vec4&, const char* = "");

	// Print Matrices
	static void PrintMatrix3x3(glm::mat3&, const char* = "");
	static void PrintMatrix4x4(glm::mat4&, const char* = "");

	// Logs
	// #TODO: log into file
	static void Log(const LOG_TYPE, const char*, const char* = "");
};
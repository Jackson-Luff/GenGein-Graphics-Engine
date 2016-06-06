#pragma once
#include <glm\fwd.hpp>

class Console
{
public:

	enum TEXT_COLOUR
	{
		LOG_SUCCESS = 0x0A,
		LOG_WARNING = 0x0E,
		LOG_ERROR	= 0x0C,
	};

	// Print Primitive Types
	static void PrintText(const char*);
	static void PrintFloat(float, const char* = "");
	static void PrintInt(int, const char* = "");

	// Print Vectors
	static void PrintVector2(glm::vec2, const char* = "");
	static void PrintVector3(glm::vec3, const char* = "");
	static void PrintVector4(glm::vec4, const char* = "");

	// Print Matrices
	static void PrintMatrix3x3(glm::mat3, const char* = "");
	static void PrintMatrix4x4(glm::mat4, const char* = "");

	// Logs
	// #TODO: log into file
	static void Log(const char*, const char* = "");
};
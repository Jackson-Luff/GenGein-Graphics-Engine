#pragma once

#include <glm\fwd.hpp>

class Console
{
public:
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

};
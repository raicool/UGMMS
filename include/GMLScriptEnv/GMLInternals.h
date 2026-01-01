#pragma once
#include "gml.h"
#include <string>

typedef GMLVar& (*GMLScriptPtr)(GMLInstance* self, GMLInstance* other, GMLVar& out, int argCount, GMLVar** args);

struct func_info
{
	uint32_t id;
	void* ptr;
};

// Contains methods for dealing with YYC GML on a low level
namespace GMLInternals 
{
	std::string __InitialSetup();

	func_info get_function_info(std::string name);
	func_info get_function_info(const char* name);

	// Call a built-in GM function using its numerical ID
	// Arguments are passed by array
	// Set the noReturn parameter to true if you don't need the function return value, otherwise it must be manually freed
	GMLVar* callGMLFunction(int functionID, int argCount, GMLVar** args, GMLVar* return_value = nullptr);
	//GMLScriptPtr getScriptPtr(int scriptID);
}
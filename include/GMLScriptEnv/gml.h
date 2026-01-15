#pragma once

#include "gml_types.h"

#include <string>

struct func_info
{
	uint32_t id;
	void* ptr;
};

typedef GMLVar& (*GMLScriptPtr)(
	IN GMLInstance* self, 
	IN GMLInstance* other, 
	OUT GMLVar& out, 
	IN int argCount, 
	IN GMLVar* args
);

static RVariableRoutine* builtin_array = nullptr;
static int32_t* builtin_count = nullptr;

std::string __gml_setup();

func_info __impl_get_func_info(
	IN std::string name
);

func_info __impl_get_func_info(
	IN const char* name
);

// Call a built-in GM function using its numerical ID
// Arguments are passed by array
GMLVar* __impl_gml_call_func(
	IN int func_id,
	IN int arg_count, 
	IN GMLVar** args
);

size_t __impl_get_builtin_variable_index(
	IN const char* name
);


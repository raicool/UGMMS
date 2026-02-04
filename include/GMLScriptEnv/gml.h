#pragma once

#include "gml_types.h"

#include <string>

struct func_info
{
	uint32_t id;
	void* ptr;
};

typedef RValue& (*PFUNC_YYGMLScript)(
	_In_ GMLInstance* self, 
	_In_ GMLInstance* other,
	_Out_ RValue& out, 
	_In_ int argCount,
	_In_ RValue* args
);

typedef void (*PFUNC_YYGML)(
	_In_ CInstance* self,
	_In_ CInstance* other
	);

using FNVariable = bool(*)(
	CInstance* Instance,
	int Index,
	RValue* Value
	);

static RVariableRoutine* builtin_array = nullptr;
static int32_t* builtin_count = nullptr;

std::string __gml_setup();

func_info __impl_get_func_info(
	_In_ std::string name
);

func_info __impl_get_func_info(
	_In_ const char* name
);

// Call a built-in GM function using its numerical ID
// Arguments are passed by array
RValue* __impl_gml_call_func(
	_In_ int func_id,
	_In_ int arg_count,
	_In_ RValue** args
);

size_t __impl_get_builtin_variable_index(
	_In_ const char* name
);


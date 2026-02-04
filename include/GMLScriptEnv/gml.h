#pragma once

#include "yoyo.h"

#include <string>

struct func_info
{
	uint32_t id;
	void* ptr;
};

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


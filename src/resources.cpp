#include "GMLScriptEnv/stdafx.h"
#include "GMLScriptEnv/resources.h"
#include "GMLScriptEnv/gml.h"

int resources_count(int func_id)
{
	int val = -1;

	for (int i = 0;; i++)
	{
		GMLVar next_id = GMLVar(i);
		GMLVar* args[] = { &next_id };
		GMLVar* exists = __impl_gml_call_func(func_id, 1, args);
			
		if (!exists) break;

		if (exists->truthy() == false) 
		{
			val = i;
			delete exists;
			break;
		}

		delete exists;
	}

	return val;
}

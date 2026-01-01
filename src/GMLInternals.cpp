#include "GMLScriptEnv/stdafx.h"
#include <vector>
#include "GMLScriptEnv/gml.h"
#include "GMLScriptEnv/GMLInternals.h"
#include "GMLScriptEnv/MemTools.h"
#include <unordered_map>
#include "GMLScriptEnv/GMLInternals.h"

// Used for getting and setting variables
namespace GMLInternals 
{
	// Map of function name -> id
	std::unordered_map<std::string, func_info>* functionIDMap = new std::unordered_map<std::string, func_info>();
	RVariableRoutine* builtin_vars = nullptr;
	size_t builtin_count = 0;

	// Function to call built-in GML functions
	GMLVar* (*GMLLegacyCall)(GMLInstance* instSelf, GMLInstance* instOther, GMLVar& gmlResult, int argumentNumber, int functionID, GMLVar** arguments) = NULL;

	//////////////////////////////////////////////////////////////////////////////
	// SETUP /////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	// Sets up functionIDMap
	std::string readFunctionInfo(void* ptr) 
	{
		// Observing a near empty compiled GM project, each function definition is stored as:
		// 64 uint8_ts: function name (remaining uint8_ts are 0'd out)
		// 4 uint8_ts: address of function?
		// 1 uint8_t: argument count?
		// 3 uint8_ts: padding (0)
		// 1 uint8_t: unknown
		// 3 uint8_ts: padding (0)
		// 4 uint8_ts of 0xFF
		// For a total of 80 uint8_ts each
		// However, the size is not consistent in some games
		// Function list ends when a zero length name is hit.
		uint8_t* funcArray = *(uint8_t**)ptr;

		// Automatically determine size of a function entry
		// Different between test project and Rivals of Aether, can't be hardcoded
		std::vector<uint8_t> entryTerminator{ 0xFF, 0xFF, 0xFF, 0xFF };
		void* ptrEntryTerminator = MemTools::scanLocal(entryTerminator, funcArray, 120);
		if (ptrEntryTerminator == NULL) 
		{
			// Failed to find entry terminator
			return "Malformed function list";
		}
		int entrySize = (int)((uint8_t*)ptrEntryTerminator + 4 - funcArray);

		uint32_t index = 0; // Current index of function being read
		while (true) 
		{
			uint8_t* entry = funcArray + entrySize * index;

			// Read the next name
			const char* name = (const char*)(entry);
			if (strlen(name) == 0) 
			{
				// No name - End of function list
				break;
			}
			else 
			{
				// Add the name to the function map
				functionIDMap->insert(
				{ 
					std::string(name),
					func_info
					{
						index,
						*(uint32_t**)(entry + 0x40)
					} 
				});
				index += 1;
			}
		}

		return "";
	}

	//GMLScriptPtr (*GetScriptAddress)(INT32 index) = nullptr;
	std::string __InitialSetup() 
	{
		// Returns a string if loading failed
		if (GMLLegacyCall != NULL) return "";

		////// Find address of callLegacyFunction
		// Contains the ASM of the function with inconsistent bits replaced with '?'
		std::vector<uint8_t> binLegacyCallFirst
{
			0x55, 0x8B, 0xEC, 0x6A, 0xFF, '?', '?', '?', '?', '?', 0x64, 0xA1, 0x00, 0x00, 0x00, 0x00,
			0x50, 0x83, 0xEC, 0x14, 0xA1, '?', '?', '?', '?', 0x33, 0xC5, 0x89, 0x45, 0xF0, 0x53, 0x56,
			0x57, 0x50, 0x8D, 0x45, 0xF4, 0x64, 0xA3, 0x00, 0x00, 0x00, 0x00, '?', 0x45, 0x18
		};

		// Search for the function
		void* legacyCallAddr = MemTools::scan(binLegacyCallFirst);
		if (legacyCallAddr == NULL) 
		{
			// Failed to find it
			return "Failed to find CallLegacyFunction";
		}
		GMLLegacyCall = (GMLVar* (*)(GMLInstance*, GMLInstance*, GMLVar&, int, int, GMLVar**))legacyCallAddr;

		////// Search for second ASM chunk containing pointer to pointer to function array
		// '?'d out bit here is the pointer we need
		std::vector<uint8_t> binLegacyCallSecond
		{
			0x03, 0x05, '?', '?', '?', '?', 0x89, 0x45,
			0xE4, 0x8B, 0xC7, 0xC1, 0xE0, 0x04, 0xE8
		};

		// Search for it starting from the end of the first chunk
		void* legacyCallChunkAddr = MemTools::scanLocal(binLegacyCallSecond, (uint8_t*)legacyCallAddr + binLegacyCallFirst.size(), 64);
		if (GMLLegacyCall == nullptr) {
			// Failed to find second halfcallGMLFunction
			GMLLegacyCall = NULL;
			return "Malformed CallLegacyFunction";
		}

		// Pull the address from the ASM
		void* funcArrayAddrBase = MemTools::readPointer((uint8_t*)legacyCallChunkAddr + 2);
		// Initialize function array
		std::string readFuncResult = readFunctionInfo(funcArrayAddrBase);
		if (readFuncResult != "") 
		{
			// Failed to load function list
			GMLLegacyCall = NULL;
			return readFuncResult;
		}

		// set up builtin vars array
// 		std::vector<uint8_t> pattern =
// 		{
// 			0x3d, 0xf4, 0x01, 0x00, 0x00, // cmp eax, 0x1f4
// 			0x75, 0x00, '?', '?' // jnz short 0x????
// 		};
// 
// 		void* ptr = MemTools::scan(pattern);
// 
// 		if (!ptr)
// 		{
// 			return "failed to find builtin variable array";
// 		}
// 
// 		uint16_t* jump_target = *(uint16_t**)((uint8_t*)ptr + 0x05);

		// Finds function used to get script pointers from id
		// This function might return some struct, needs more research before it can be used
		/*std::vector<uint8_t> getScrPtrDat{
			 0x8B, 0x4C, 0x24, 0x04,
			 0x85, 0xC9,
			 0x78, 0x11,
			 0x3B, 0x0D, '?', '?', '?', '?',
			 0x7D, 0x09,
			 0xA1, '?', '?', '?', '?',
			 0x8B, 0x04, 0x88,
			 0xC3,
			 0x33, 0xC0,
			 0xC3
		};
		GetScriptAddress = (GMLScriptPtr(*)(int))MemTools::scan(getScrPtrDat);*/
		return "";
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// METHODS ///////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	func_info get_function_info(std::string name) 
	{
		auto t = functionIDMap->find(name);
		if (t == functionIDMap->end()) return {};
		return t->second;
	}

    func_info get_function_info(const char* name)
	{
        return get_function_info(std::string(name));
    }

	GMLVar* GMLRetDummy = new GMLVar();
	GMLVar* callGMLFunction(int functionID, int argCount, GMLVar** args, GMLVar* return_value)
	{
		if (GMLLegacyCall == NULL) return NULL;
        if (return_value == NULL) 
		{
            return_value = GMLRetDummy;
        }
        return_value->freeValue();
        GMLLegacyCall(NULL, NULL, *return_value, argCount, functionID, args);
        return return_value;
	}

	/*GMLScriptPtr getScriptPtr(int id) {
		if (GetScriptAddress != nullptr) {
			return GetScriptAddress(id);
		} else {
			return nullptr;
		}
	}*/

	size_t get_builtin_variable_index(const char* name)
	{
		return 0;
	}
}

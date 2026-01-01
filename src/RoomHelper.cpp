#include "GMLScriptEnv/stdafx.h"
#include "GMLScriptEnv/GMLInternals.h"
#include "GMLScriptEnv/RoomHelper.h"
#include "GMLScriptEnv/HelperHelper.h"
#include "GMLScriptEnv/MemTools.h"

#include <vector>

namespace RoomHelper 
{
	using func_roomdata = CRoom*(*)(int index);

	func_roomdata room_data = nullptr;

	int roomCount = -1;

	void __InitialSetup() 
	{
		find_room_data();

		if (roomCount != -1) return;
		func_info room_exists = GMLInternals::get_function_info("room_exists");
		roomCount = HelperHelper::countResource(room_exists.id);
	}

	int getRoomCount() 
	{
		return roomCount;
	}

	void* find_room_data()
	{
		func_info room_instance_clear = GMLInternals::get_function_info("room_instance_clear");

		std::vector<uint8_t> pattern =
		{
			0xe8, '?', '?', '?', '?',	// call <room_data>
			0x83, 0xc4, 0x0c,			// add esp, 0xc
			0x85, 0xc0,					// test eax, eax
		};

		void* ptr = MemTools::scanLocal(pattern, room_instance_clear.ptr, 0x80);
		
		if (ptr)
		{
			// found the call instruction, now get the absolute address of room_data
			room_data = (func_roomdata)MemTools::absolute_address(ptr, *((uint32_t**)((uint8_t*)ptr + 0x01)), 0x05);
		}

		return room_data;
	}

	CRoom* __impl_get_room_by_index(uint32_t index)
	{
		return room_data(index);
	}
}

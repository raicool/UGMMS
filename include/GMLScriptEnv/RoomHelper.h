#pragma once

#include "GMLInternals.h"

namespace RoomHelper 
{
	void __InitialSetup();

	// Returns the number of built-in rooms the game has
	int getRoomCount();

	void* find_room_data();
	CRoom* __impl_get_room_by_index(uint32_t index);
}

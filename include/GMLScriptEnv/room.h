#pragma once

#include "gml.h"

namespace room 
{
	void __setup();

	// Returns the number of built-in rooms the game has
	int get_room_count();

	void* find_room_data();

	CRoom* __impl_get_room_by_index(uint32_t index);
}

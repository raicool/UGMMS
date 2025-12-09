#include "GMLScriptEnv/stdafx.h"
#include "GMLScriptEnv/GMLInternals.h"
#include "GMLScriptEnv/RoomHelper.h"
#include "GMLScriptEnv/HelperHelper.h"

namespace RoomHelper {
	int roomCount = -1;

	void __InitialSetup() {
		if (roomCount != -1) return;
		roomCount = HelperHelper::countResource(GMLInternals::getFunctionID("room_exists"));
	}

	int getRoomCount() {
		return roomCount;
	}
}

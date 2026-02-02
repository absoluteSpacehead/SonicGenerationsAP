#pragma once

// im really not familiar with BlueBlur's styling so im not putting this in there myself
// this is just what i need to get shit working..

enum class EEmeraldState : int32_t
{
	eEmeraldState_Inactive = 0,
	eEmeraldState_Ready = 1,
	eEmeraldState_Active = 2,
	eEmeraldState_Collected = 3,
	eEmeraldState_Gear1 = 4,
	eEmeraldState_Gear2 = 5,
	eEmeraldState_Gear3 = 6,
	eEmeraldState_Gear4 = 7,
	eEmeraldState_Gear5 = 8,
	eEmeraldState_Gear6 = 9,
	eEmeraldState_Gear7 = 10
};

class CChaosEmeraldPlayableMenu
{
public:
	char pad0[260];				// 0x0
	int32_t EmeraldIndex;		// 0x104
	char pad1[24];				// 0x108
	EEmeraldState EmeraldState;	// 0x120
};
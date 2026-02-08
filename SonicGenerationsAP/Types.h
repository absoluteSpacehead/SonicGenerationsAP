#pragma once

// im really not familiar with BlueBlur's styling so im not putting this in there myself
// this is just what i need to get shit working..

class CChaosEmeraldPlayableMenu
{
public:
	char pad0[260];													// 0x0
	int32_t EmeraldIndex;											// 0x104
	char pad1[24];													// 0x108
	Sonic::CGameParameter::SSaveData::EEmeraldState EmeraldState;	// 0x120
};

class CObjBossGateKeyForSetObject
{
public:
	char pad0[260];												// 0x0
	int32_t KeyIndex;											// 0x104
	Sonic::CGameParameter::SSaveData::EBossKeyState KeyState;	// 0x108
};

// i never actually checked if this is in fact the type referenced in func 0x569AE0, i just kinda assumed it was because it was in the vtable for that class
// so if i can be bothered ill double check this and correct it if its wrong lol
class CSequenceElementTakeEmerald
{
public:
	char pad0[4];		// 0x0
	int32_t EmeraldIndex;	// 0x4
};
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

class CSequenceElementTakeEmerald
{
public:
	char pad0[4];			// 0x0
	int32_t EmeraldIndex;	// 0x4
};

class CObjRedMedal
{
public:
	char pad0[184];		 // 0x0
	int32_t FieldB8;	 // 0xB8
	int32_t FieldBC;	 // 0xBC
	char pad1[56];		 // 0xC0
	int32_t* FieldF8;	 // 0xF8
	char pad2[44];		 // 0xFC
	uint32_t MedalIndex; // 0x128
};
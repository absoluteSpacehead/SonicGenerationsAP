#include <iostream>
#include "Archipelago.h"
#include "Config.h"
#include <LostCodeLoader.h>

Sonic::CGameParameter::SSaveData* pSaveData = nullptr;

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	return TRUE;
}

#pragma region Chaos Emerald
uint32_t __fastcall DecideSavedEmeraldValue(int32_t index)
{
	return CArchipelagoData::IsItemRecieved(EItem::eItemEGreen + index) ? Sonic::CGameParameter::SSaveData::EEmeraldState::eEmeraldState_Collected : 0;
}

#pragma region Playable Menu
HOOK(void, __fastcall, ChaosEmeraldTouched, 0xEF8A60, CChaosEmeraldPlayableMenu* pThis)
{
	if (pThis->EmeraldState == Sonic::CGameParameter::SSaveData::EEmeraldState::eEmeraldState_Active)
		CArchipelagoData::TryCheckLocation(ELocation::eLocationEGreen + pThis->EmeraldIndex);

	originalChaosEmeraldTouched(pThis);
}

uint32_t ChaosEmeraldTouched_ASMReturnAddress = 0xEF7E02;
void __declspec(naked) ChaosEmeraldTouched_ASM()
{
	__asm
	{
		push eax
		push ecx
		mov ecx, dword ptr[edi + 0x104]
		call[DecideSavedEmeraldValue]

		mov edx, eax
		pop ecx
		pop eax

		jmp[ChaosEmeraldTouched_ASMReturnAddress]
	}
}

uint32_t __fastcall DecideShowEmerald(int32_t index)
{
	return pSaveData->Emeralds[index] >= 3 && !CArchipelagoData::IsLocationChecked(ELocation::eLocationEGreen + index) ? Sonic::CGameParameter::SSaveData::EEmeraldState::eEmeraldState_Inactive : pSaveData->Emeralds[index];
}

uint32_t ChaosEmeraldInit_ASMReturnAddress = 0xEF859B;
void __declspec(naked) ChaosEmeraldInit_ASM()
{ 
	__asm
	{
		mov eax, dword ptr[esi + 0xa4]

		push eax
		push ecx
		mov ecx, ebx
		call[DecideShowEmerald]

		mov dword ptr[esi + 0x120], eax

		pop ecx
		pop eax
		jmp[ChaosEmeraldInit_ASMReturnAddress]
	}
}
#pragma endregion

#pragma region Red Emerald (Planet Wisp Cutscene)
HOOK(void, __fastcall, RedEmeraldCollect, 0x569AE0, CSequenceElementTakeEmerald* pThis)
{
	CArchipelagoData::TryCheckLocation(ELocation::eLocationERed);

	originalRedEmeraldCollect(pThis);
}

uint32_t RedEmeraldCollect_ASMReturnAddress = 0x569B15;
void __declspec(naked) RedEmeraldCollect_ASM()
{
	__asm
	{
		push eax
		push edx
		call[DecideSavedEmeraldValue]
		pop edx

		test eax, eax
		jz NotCollected

		pop eax
		mov dword ptr[eax + ecx * 0x4 + 0xA048], 0x3
		jmp[RedEmeraldCollect_ASMReturnAddress]

		NotCollected:
		pop eax
		mov dword ptr[eax + ecx * 0x4 + 0xA048], 0x0
		jmp[RedEmeraldCollect_ASMReturnAddress]
	}
}
#pragma endregion
#pragma endregion

#pragma region Boss Key
HOOK(void, __fastcall, BossKeyTouched, 0x4DBA30, CObjBossGateKeyForSetObject* pThis)
{
	if (pThis->KeyState == Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Collectable)
		CArchipelagoData::TryCheckLocation(ELocation::eLocationBKGHZ + pThis->KeyIndex / 2);

	originalBossKeyTouched(pThis);
}

uint32_t __fastcall DecideSavedBossKeyValue(int32_t index)
{
	return CArchipelagoData::IsItemRecieved(EItem::eItemBKGHZ + index / 2) ? Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Collected : Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Active;
}

uint32_t BossKeyTouched_ASMReturnAddress = 0x4DBDBD;
void __declspec(naked) BossKeyTouched_ASM()
{
	__asm
	{
		push ecx
		push edx
		push eax
		mov ecx, eax
		call[DecideSavedBossKeyValue]

		mov ecx, eax
		pop eax
		pop edx
		
		mov dword ptr[edx + eax * 0x4 + 0xA000], ecx
		mov dword ptr[edx + eax * 0x4 + 0xA004], ecx

		pop ecx

		jmp[BossKeyTouched_ASMReturnAddress]
	}
}

uint32_t __fastcall DecideShowBossKey(int32_t index)
{
	return pSaveData->BossKeys[index] >= 3 && !CArchipelagoData::IsLocationChecked(ELocation::eLocationBKGHZ + index / 2) ? Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Active : pSaveData->BossKeys[index];
}

uint32_t BossKeyCheck_ASMReturnAddress = 0x4DBEF3;
void __declspec(naked) BossKeyCheck_ASM()
{
	__asm
	{
		push edx
		call[DecideShowBossKey]
		pop edx

		mov dword ptr[edi + 0x108], eax

		jmp[BossKeyCheck_ASMReturnAddress]
	}
}

bool __fastcall DecideSetBossKeyActive(int32_t index)
{
	return !CArchipelagoData::IsLocationChecked(ELocation::eLocationBKGHZ + index / 2);
}

// this hook does mean that you can briefly un-collect the boss key.. but it restores itself when you pick it up if you had previously received it so i think thats OK
uint32_t SetBossKeyActiveOnMissionClear_ASMReturnAddress_OK = 0xD71774;
uint32_t SetBossKeyActiveOnMissionClear_ASMReturnAddress_NG = 0xD717BE;
void __declspec(naked) SetBossKeyActiveOnMissionClear_ASM()
{
	__asm
	{
		push ecx
		mov ecx, ebx
		call[DecideSetBossKeyActive]
		pop ecx

		test eax,eax
		jz NG
		jmp[SetBossKeyActiveOnMissionClear_ASMReturnAddress_OK]

		NG:
		jmp[SetBossKeyActiveOnMissionClear_ASMReturnAddress_NG]
	}
}
#pragma endregion

// TODO: disable the thing the game does where you *have* to play each level in order (per sonic) [cont.]
// for example: you cannot play city escape act 1 before playing all act 1s before it
// this makes sense in vanilla, not here tho lol
// this also isnt per era, so you cant skip seaside hill and play crisis city
// but it does NOT apply to bosses, you can play bosses before playing all the levels
#pragma region Act Clear
/*
 * SSaveData stages go:
 * 0 - 17 (ghz1->pla2)
 * cnz1 is on 18
 * cnz2 is probably on 19
 * bms is on 20
 * bsd is on 21
 * bsl is probably on 22
 * bde is on 23
 * bpc is on 24
 * bne is probably on 25
 * blb is probably on 26
 * missions start on 27
 * bms-1 is on 117
 * blb-1 is probably on 123
 * no idea whats taking the remaining of the 142 slots
 */

void __fastcall OnStageClear(uint32_t index)
{
	if (index >= 0 && index <= 17)
		CArchipelagoData::TryCheckLocation(ELocation::eLocationClearGHZ1 + index);
	else if (index >= 20 && index <= 25)
		CArchipelagoData::TryCheckLocation(ELocation::eLocationClearBMS + (index - 20));
	else if (index == 26)
	{
		if (CArchipelagoData::TryCheckLocation(ELocation::eLocationClearBLB))
			AP_StoryComplete();
	}
}

uint32_t StageClear_ASMReturnAddress_OK = 0xD585F1;
uint32_t StageClear_ASMReturnAddress_NG = 0xD58661;
void __declspec(naked) StageClear_ASM()
{
	__asm
	{
		push ecx
		push eax
		mov ecx, eax
		call[OnStageClear]
		pop eax
		pop ecx

		cmp eax, 0x8E
		jae NG

		jmp[StageClear_ASMReturnAddress_OK]

		NG:
		jmp[StageClear_ASMReturnAddress_NG]
	}
}
#pragma endregion

void InitHooks()
{
	INSTALL_HOOK(ChaosEmeraldTouched);
	INSTALL_HOOK(RedEmeraldCollect);
	INSTALL_HOOK(BossKeyTouched);
	
	WRITE_JUMP(0xEF7DFC, ChaosEmeraldTouched_ASM);
	WRITE_JUMP(0xEF8583, ChaosEmeraldInit_ASM);
	WRITE_JUMP(0x569B0A, RedEmeraldCollect_ASM);
	WRITE_JUMP(0x4DBDAC, BossKeyTouched_ASM);
	WRITE_JUMP(0x4DBEED, BossKeyCheck_ASM);
	WRITE_JUMP(0xD7176B, SetBossKeyActiveOnMissionClear_ASM);
	WRITE_JUMP(0xD585EA, StageClear_ASM);
}

bool InitAP()
{
	if (!CConfiguration::LoadConfig())
		return false;

	AP_Init(CConfiguration::GetAPAddress().c_str(), "Sonic Generations", CConfiguration::GetAPSlotName().c_str(), CConfiguration::GetAPPassword().c_str());

	AP_SetItemClearCallback(CArchipelagoData::ClearData);
	AP_SetItemRecvCallback(CArchipelagoData::OnItemRecieved);
	AP_SetLocationCheckedCallback(CArchipelagoData::OnLocationChecked);

	AP_Start();
	return true;
}

extern "C" __declspec(dllexport) void OnFrame()
{
	// todo: queue prompts and display them when in white space
	if (AP_IsInit())
	{
		if (AP_IsMessagePending())
		{
			//AP_Message* msg = AP_GetLatestMessage();
			AP_ClearLatestMessage();
		}
	}
	else
	{
		if (Sonic::CGameDocument::GetInstance()->m_pMember->m_StageName.length() != 0 && !CArchipelagoData::HasTriedInit)
		{
			CArchipelagoData::HasTriedInit = true;

			InitAP();
		}
	}

	Hedgehog::Base::TSynchronizedPtr<Sonic::CApplicationDocument> pDoc = Sonic::CApplicationDocument::GetInstance();

	if (!pDoc)
		return;

	if (pDoc->m_pMember->m_spGameParameter->m_pSaveData == pSaveData)
		return;

	pSaveData = pDoc->m_pMember->m_spGameParameter->m_pSaveData;
	printf("New save data address: %p\n", pDoc->m_pMember->m_spGameParameter->m_pSaveData);
}

extern "C" __declspec(dllexport) void Init(ModInfo* info)
{
	std::string path = info->CurrentMod->Path;
	size_t pos = path.find_last_of("\\/");
	if (pos != std::string::npos)
		path.erase(pos + 1);
	path += "Config.ini";
	CConfiguration::ConfigPath = path;

	InitHooks();
}
#include <iostream>
#include "Archipelago.h"
#include "Config.h"
#include "Utils.h"
#include "APChaosEmerald.h"
#include "APBossKey.h"
#include "Globals.h"
#include <LostCodeLoader.h>

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	return TRUE;
}

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
	if (index >= 0 && index <= 17) // ghz1->pla2
		Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationClearGHZ1 + index);
	else if (index >= 20 && index <= 25) // bms -> bne
		Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationClearBMS + (index - 20));
	else if (index == 26) // blb
	{
		if (Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationClearBLB) && !Archipelago::pGameSettings.HardBosses)
			AP_StoryComplete();
	}
	else if (index >= 27 && index <= 116 && Archipelago::pGameSettings.Challenges) // ghz11->pla25
		Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationClearGHZ11 + (index - 27));
	else if (index >= 117 && index <= 122 && Archipelago::pGameSettings.HardBosses) // bms1->bne1
		Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationClearBMS1 + (index - 117));
	else if (index == 123 && Archipelago::pGameSettings.HardBosses) // blb1
	{
		if (Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationClearBLB1))
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

// 0x10840 -- HE1ML CSaveLoadTestPC::SaveContentsRead hook.
// it completely replaces the original func and im pretty sure it does it *after* loading all the mods, so we gotta hook that instead of re-hooking the original
uint32_t HEMLSaveContentsReadHook_Offset = 0x10840;
HOOK(bool, __fastcall, HEMLSaveContentsReadHook, Utils::Offset<uint32_t>(L"dinput8.dll", HEMLSaveContentsReadHook_Offset), void* ecx, void* edx, void* buffer, size_t bufsize)
{
	if (!Archipelago::HasTriedInit)
		Archipelago::InitAP();

	return originalHEMLSaveContentsReadHook(ecx, edx, buffer, bufsize);
}

// prevents selecting menu options until authenticated.
// todo: make this only apply to new game / continue
uint32_t MenuConfirmPressed_ASMReturnAddress = 0x572c23;
void __declspec(naked) MenuConfirmPressed_ASM()
{
	__asm
	{
		push eax
		push ecx
		call[AP_GetConnectionStatus]
		cmp eax, 0x2
		pop ecx
		pop eax
		jz OK

		pop edi
		pop esi
		pop ebp
		pop ebx
		mov esp,ebp
		pop ebp
		ret

		OK:
		cmp dword ptr[ebx + 0x1EA], 0x0
		jmp[MenuConfirmPressed_ASMReturnAddress]
	}
}

void InitHooks()
{
	APChaosEmerald::InstallHooks();
	APBossKey::InstallHooks();

	INSTALL_HOOK(HEMLSaveContentsReadHook);

	WRITE_JUMP(0xD585EA, StageClear_ASM);
	WRITE_JUMP(0x572C1C, MenuConfirmPressed_ASM);
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

	Hedgehog::Base::TSynchronizedPtr<Sonic::CApplicationDocument> pDoc = Sonic::CApplicationDocument::GetInstance();

	if (!pDoc)
		return;

	if (pDoc->m_pMember->m_spGameParameter->m_pSaveData == Globals::pSaveData)
		return;

	Globals::pSaveData = pDoc->m_pMember->m_spGameParameter->m_pSaveData;
	printf("New save data address: %p\n", pDoc->m_pMember->m_spGameParameter->m_pSaveData);
}

extern "C" __declspec(dllexport) void Init(ModInfo* info)
{
	std::string path = info->CurrentMod->Path;
	size_t pos = path.find_last_of("\\/");
	if (pos != std::string::npos)
		path.erase(pos + 1);
	path += "Config.ini";
	Config::ConfigPath = path;

	InitHooks();
}
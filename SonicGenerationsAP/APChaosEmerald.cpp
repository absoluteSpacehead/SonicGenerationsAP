#include "APChaosEmerald.h"
#include "Archipelago.h"
#include "Globals.h"

uint32_t __fastcall DecideSavedEmeraldValue(int32_t index)
{
	return Archipelago::IsItemRecieved(Archipelago::EItem::eItemEGreen + index) ? 3 : 0;
}

HOOK(void, __fastcall, ChaosEmeraldTouched, 0xEF8A60, CChaosEmeraldPlayableMenu* pThis)
{
	if (pThis->EmeraldState == 2)
		Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationEGreen + pThis->EmeraldIndex);

	originalChaosEmeraldTouched(pThis);
}

uint32_t ChaosEmeraldTouched_ASMReturnAddress = 0xEF7DDB;
void __declspec(naked) ChaosEmeraldTouched_ASM()
{
	__asm
	{
		push edx
		push eax
		push ecx
		mov ecx, esi
		call[DecideSavedEmeraldValue]
		
		mov dword ptr[edi + 0x120], eax
		pop ecx
		pop eax
		pop edx

		jmp[ChaosEmeraldTouched_ASMReturnAddress]
	}
}

uint32_t __fastcall DecideShowEmerald(int32_t index)
{
	return Globals::pSaveData->Emeralds[index] >= 3 && !Archipelago::IsLocationChecked(Archipelago::ELocation::eLocationEGreen + index) ? 0 : Globals::pSaveData->Emeralds[index];
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

HOOK(void, __fastcall, RedEmeraldCollect, 0x569AE0, CSequenceElementTakeEmerald* pThis)
{
	Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationERed);

	originalRedEmeraldCollect(pThis);
}

uint32_t RedEmeraldCollect_ASMReturnAddress = 0x569B15;
void __declspec(naked) RedEmeraldCollect_ASM()
{
	__asm
	{
		push edx
		push eax
		call[DecideSavedEmeraldValue]

		mov edx, eax
		pop eax

		mov dword ptr[eax + ecx * 0x4 + 0xA048], edx

		pop edx
		jmp[RedEmeraldCollect_ASMReturnAddress]
	}
}

void APChaosEmerald::InstallHooks()
{
	INSTALL_HOOK(ChaosEmeraldTouched);
	INSTALL_HOOK(RedEmeraldCollect);

	WRITE_JUMP(0xEF7DD1, ChaosEmeraldTouched_ASM);
	WRITE_JUMP(0xEF8583, ChaosEmeraldInit_ASM);
	WRITE_JUMP(0x569B0A, RedEmeraldCollect_ASM);
}
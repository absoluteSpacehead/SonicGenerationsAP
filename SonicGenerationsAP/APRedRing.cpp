#include "APRedRing.h"
#include "Archipelago.h"
#include "Globals.h"

HOOK(void, __fastcall, MedalCollected, 0x11A9BA0, CObjRedMedal* pThis)
{
	Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationRedRingGHZ11 + pThis->MedalIndex);

	originalMedalCollected(pThis);
}

// Skip setting the medal in the save file
// ebx is xor'd because another part immediately after reuses its 0 value
uint32_t MedalCollected_ASMReturnAddress = 0x11A9C25;
void __declspec(naked) MedalCollected_ASM()
{
	__asm
	{
		xor ebx,ebx
		jmp[MedalCollected_ASMReturnAddress]
	}
}

uint32_t __fastcall DecideShowMedal(int32_t index)
{
	return Archipelago::IsLocationChecked(Archipelago::ELocation::eLocationRedRingGHZ11 + index);
}

uint32_t MedalInit_ASMReturnAddress = 0x11A9EA9;
void __declspec(naked) MedalInit_ASM()
{
	__asm
	{
		push ecx
		mov ecx, esi
		call[DecideShowMedal]
		pop ecx

		test eax,eax
		jmp[MedalInit_ASMReturnAddress]
	}
}

void APRedRing::InstallHooks()
{
	INSTALL_HOOK(MedalCollected);

	WRITE_JUMP(0x11A9E97, MedalInit_ASM);
	WRITE_JUMP(0x11A9C0B, MedalCollected_ASM);
	
	// patch out check for ghz2 completion so you can get them from a fresh file
	WRITE_NOP(0x11A9ECB, 2);
}
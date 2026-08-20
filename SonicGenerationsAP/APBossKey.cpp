#include "APBossKey.h"
#include "Archipelago.h"
#include "Globals.h"

HOOK(void, __fastcall, BossKeyTouched, 0x4DBA30, CObjBossGateKeyForSetObject* pThis)
{
	if (pThis->KeyState == 2)
		Archipelago::TryCheckLocation(Archipelago::ELocation::eLocationBKGHZ + pThis->KeyIndex / 2);

	originalBossKeyTouched(pThis);
}

uint32_t __fastcall DecideSavedBossKeyValue(int32_t index)
{
	return Archipelago::IsItemRecieved(Archipelago::EItem::eItemBKGHZ + index / 2) ? 3 : 1;
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
	return Globals::pSaveData->BossKeys[index] == 3 && !Archipelago::IsLocationChecked(Archipelago::ELocation::eLocationBKGHZ + index / 2) ? 1 : Globals::pSaveData->BossKeys[index];
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
	return !Archipelago::IsLocationChecked(Archipelago::ELocation::eLocationBKGHZ + index / 2);
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

		test eax, eax
		jz NG
		jmp[SetBossKeyActiveOnMissionClear_ASMReturnAddress_OK]

		NG:
		jmp[SetBossKeyActiveOnMissionClear_ASMReturnAddress_NG]
	}
}

void APBossKey::InstallHooks()
{
	INSTALL_HOOK(BossKeyTouched);

	WRITE_JUMP(0x4DBDAC, BossKeyTouched_ASM);
	WRITE_JUMP(0x4DBEED, BossKeyCheck_ASM);
	WRITE_JUMP(0xD7176B, SetBossKeyActiveOnMissionClear_ASM);
}
#include <iostream>

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	return TRUE;
}

HOOK(void, , ChaosEmeraldCollected, 0xEF7DC0)
{
	// do nothing
	printf("ChaosEmeraldCollected called, blocking\n");
	return;
}

// todo: replace this with an asm hook at some point after EF8595 (assigning state to the object)
HOOK(void*, __fastcall, ChaosEmeraldInit, 0xEF8510, CChaosEmeraldPlayableMenu* This, void* Edx, void* A1, void* A2)
{
	printf("ChaosEmeraldInit called with emerald state %d (thisptr %p, index %d)\n", This->EmeraldState, This, This->EmeraldIndex);
	return originalChaosEmeraldInit(This, Edx, A1, A2);
}

Sonic::CGameParameter::SSaveData* lastKnown = nullptr;

extern "C" __declspec(dllexport) void OnFrame()
{
	Hedgehog::Base::TSynchronizedPtr<Sonic::CApplicationDocument> pDoc = Sonic::CApplicationDocument::GetInstance();

	if (!pDoc || pDoc->m_pMember->m_spGameParameter->m_pSaveData == lastKnown)
		return;

	lastKnown = pDoc->m_pMember->m_spGameParameter->m_pSaveData;
	printf("New save data address: %p\n", pDoc->m_pMember->m_spGameParameter->m_pSaveData);
}

extern "C" __declspec(dllexport) void Init()
{
	INSTALL_HOOK(ChaosEmeraldCollected);
	INSTALL_HOOK(ChaosEmeraldInit);
}
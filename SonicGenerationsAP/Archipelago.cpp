#include "Archipelago.h"
#include "Config.h"
#include "Utils.h"

void Archipelago::ClearData()
{
	for (int i = 0; i < ELocation::nLocation; i++) LocationsChecked[i] = false;
	for (int i = 0; i < EItem::nItem; i++) ItemsCollected[i] = false;
}

void Archipelago::OnItemRecieved(int id, bool alert)
{
	if (id >= EItem::nItem)
	{
		printf("invalid item recieved? %d\n", id);
		return;
	}

	// progression
	printf("item recieved %d\n", id);
	Sonic::CGameParameter::SSaveData* pSaveData = Sonic::CApplicationDocument::GetInstance()->m_pMember->m_spGameParameter->m_pSaveData;

	// emeralds
	if (id >= EItem::eItemEGreen && id <= EItem::eItemEWhite)
	{
		ItemsCollected[id] = true;

		if (pSaveData->Emeralds[id - 1] != Sonic::CGameParameter::SSaveData::EEmeraldState::eEmeraldState_Active)
			pSaveData->Emeralds[id - 1] = Sonic::CGameParameter::SSaveData::EEmeraldState::eEmeraldState_Collected;
	}

	// boss keys
	if (id >= EItem::eItemBKGHZ && id <= EItem::eItemBKPLA)
	{
		ItemsCollected[id] = true;
		int keyId = id - EItem::eItemBKGHZ;

		// for some reason each one is duplicated, i assume one for modern one for classic
		// but you only need to collect one per zone...
		// maybe they were planning to make it 6 keys per boss. jesus christ
		if (pSaveData->BossKeys[keyId * 2] != Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Collectable)
		{
			pSaveData->BossKeys[keyId * 2] = Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Collected;
			pSaveData->BossKeys[(keyId * 2) + 1] = Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Collected;
		}
	}

	// non-progression
	if (!alert)
	{
		printf("item resent %d\n", id);
		// mark as collected but ignore otherwise
		if (id != EItem::eItemNothing)
			ItemsCollected[id] = true;
		return;
	}

	// filler
	if (id == EItem::eItemNothing) {}
}

void Archipelago::OnLocationChecked(int id)
{
	if (id >= 0 && id < ELocation::nLocation)
	{
		LocationsChecked[id] = true;
	}

	// probably shouldnt silently ignore.. i dont care tho
}

bool __fastcall Archipelago::IsLocationChecked(int id)
{
	if (id < 0 || id >= ELocation::nLocation) return false;

	return LocationsChecked[id];
}

bool __fastcall Archipelago::IsItemRecieved(int id)
{
	if (id < 0 || id >= EItem::nItem) return false;

	return ItemsCollected[id];
}

bool Archipelago::TryCheckLocation(int id)
{
	if (!AP_IsInit()) return false;

	if (id < 0 || id >= ELocation::nLocation) return false;

	if (LocationsChecked[id]) return false;

	LocationsChecked[id] = true;
	AP_SendItem(id);
	return true;
}

bool Archipelago::InitAP()
{
	if (!Config::LoadConfig())
		return false;

	HasTriedInit = true;

	AP_Init(Utils::Trim(Config::GetAPAddress()).c_str(), "Sonic Generations", Config::GetAPSlotName().c_str(), Config::GetAPPassword().c_str());

	AP_SetItemClearCallback(Archipelago::ClearData);
	AP_SetItemRecvCallback(Archipelago::OnItemRecieved);
	AP_SetLocationCheckedCallback(Archipelago::OnLocationChecked);

	AP_RegisterSlotDataIntCallback("hard_bosses", &SetHardBosses);
	AP_RegisterSlotDataIntCallback("challenges", &SetChallenges);

	AP_Start();
	return true;
}

void Archipelago::SetHardBosses(int enabled)
{
	pGameSettings.HardBosses = enabled;
}

void Archipelago::SetChallenges(int enabled)
{
	pGameSettings.Challenges = enabled;
}
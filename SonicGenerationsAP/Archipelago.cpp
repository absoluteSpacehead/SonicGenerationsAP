#include "Archipelago.h"

void CArchipelagoData::ClearData()
{
	for (int i = 0; i < ELocation::nLocation; i++) LocationsChecked[i] = false;
	for (int i = 0; i < EItem::nItem; i++) ItemsCollected[i] = false;
}

void CArchipelagoData::OnItemRecieved(int id, bool alert)
{
	if (!ItemsCollected[id])
	{
		printf("item recieved %d\n", id);
		Sonic::CGameParameter::SSaveData* pSaveData = Sonic::CApplicationDocument::GetInstance()->m_pMember->m_spGameParameter->m_pSaveData;

		// emeralds
		if (id >= eItemEGreen && id <= eItemEWhite)
		{
			ItemsCollected[id] = true;
			pSaveData->Emeralds[id - 1] = Sonic::CGameParameter::SSaveData::EEmeraldState::eEmeraldState_Collected;
		}

		// boss keys
		if (id >= eItemBKGHZ && id <= eItemBKPLA)
		{
			ItemsCollected[id] = true;
			int keyId = id - eItemBKGHZ;

			// for some reason each one is duplicated, i assume one for modern one for classic
			// but you only need to collect one per zone...
			// maybe they were planning to make it 6 keys per boss. jesus christ
			pSaveData->BossKeys[keyId * 2] = Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Collected;
			pSaveData->BossKeys[(keyId * 2) + 1] = Sonic::CGameParameter::SSaveData::EBossKeyState::eBossKeyState_Collected;
		}
		
		// filler
		if (id == eItemNothing) {}
	}
	else
		return;
}

void CArchipelagoData::OnLocationChecked(int id)
{
	if (id >= 0 && id < ELocation::nLocation)
	{
		LocationsChecked[id] = true;
	}

	// probably shouldnt silently ignore.. i dont care tho
}

bool __fastcall CArchipelagoData::IsLocationChecked(int id)
{
	if (id < 0 || id >= ELocation::nLocation) return false;

	return LocationsChecked[id];
}

bool __fastcall CArchipelagoData::IsItemRecieved(int id)
{
	if (id < 0 || id >= EItem::nItem) return false;

	return ItemsCollected[id];
}

bool CArchipelagoData::TryCheckLocation(int id)
{
	if (!AP_IsInit()) return false;

	if (id < 0 || id >= ELocation::nLocation) return false;

	if (LocationsChecked[id]) return false;

	LocationsChecked[id] = true;
	AP_SendItem(id);
	return true;
}
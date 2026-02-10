#pragma once

enum ELocation
{
	eLocationEGreen = 1,
	eLocationEPurple,
	eLocationEBlue,
	eLocationEYellow,
	eLocationERed,
	eLocationECyan,
	eLocationEWhite,

	eLocationBKGHZ = 50,
	eLocationBKCPZ,
	eLocationBKSSZ,
	eLocationBKSPH,
	eLocationBKCTE,
	eLocationBKSSH,
	eLocationBKCSC,
	eLocationBKEUC,
	eLocationBKPLA,

	eLocationClearGHZ1 = 100,
	eLocationClearGHZ2,
	eLocationClearCPZ1,
	eLocationClearCPZ2,
	eLocationClearSSZ1,
	eLocationClearSSZ2,
	eLocationClearSPH1,
	eLocationClearSPH2,
	eLocationClearCTE1,
	eLocationClearCTE2,
	eLocationClearSSH1,
	eLocationClearSSH2,
	eLocationClearCSC1,
	eLocationClearCSC2,
	eLocationClearEUC1,
	eLocationClearEUC2,
	eLocationClearPLA1,
	eLocationClearPLA2,
	eLocationClearBMS,
	eLocationClearBSD,
	eLocationClearBSL,
	eLocationClearBDE,
	eLocationClearBPC,
	eLocationClearBNE,
	eLocationClearBLB,

	nLocation
};

enum EItem
{
	eItemEGreen = 1,
	eItemEPurple,
	eItemEBlue,
	eItemEYellow,
	eItemERed,
	eItemECyan,
	eItemEWhite,

	eItemNothing = 8,

	eItemBKGHZ = 50,
	eItemBKCPZ,
	eItemBKSSZ,
	eItemBKSPH,
	eItemBKCTE,
	eItemBKSSH,
	eItemBKCSC,
	eItemBKEUC,
	eItemBKPLA,

	nItem
};

class CArchipelagoData
{
private:
	inline static bool LocationsChecked[ELocation::nLocation];
	inline static bool ItemsCollected[EItem::nItem];

public:
	static bool __fastcall IsLocationChecked(int id);
	static bool TryCheckLocation(int id);
	static bool __fastcall IsItemRecieved(int id);
	static void ClearData();
	static void OnItemRecieved(int id, bool alert);
	static void OnLocationChecked(int id);

	inline static bool HasTriedInit;
};
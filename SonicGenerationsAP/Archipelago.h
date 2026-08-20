#pragma once

namespace Archipelago
{
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
	
	inline bool LocationsChecked[ELocation::nLocation];
	inline bool ItemsCollected[EItem::nItem];

	bool __fastcall IsLocationChecked(int id);
	bool TryCheckLocation(int id);
	bool __fastcall IsItemRecieved(int id);
	void ClearData();
	void OnItemRecieved(int id, bool alert);
	void OnLocationChecked(int id);

	bool InitAP();

	inline bool HasTriedInit;
}
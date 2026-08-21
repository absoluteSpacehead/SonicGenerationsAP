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

		eLocationClearGHZ11,
		eLocationClearGHZ12,
		eLocationClearGHZ13,
		eLocationClearGHZ14,
		eLocationClearGHZ15,
		eLocationClearGHZ21,
		eLocationClearGHZ22,
		eLocationClearGHZ23,
		eLocationClearGHZ24,
		eLocationClearGHZ25,

		eLocationClearCPZ11,
		eLocationClearCPZ12,
		eLocationClearCPZ13,
		eLocationClearCPZ14,
		eLocationClearCPZ15,
		eLocationClearCPZ21,
		eLocationClearCPZ22,
		eLocationClearCPZ23,
		eLocationClearCPZ24,
		eLocationClearCPZ25,

		eLocationClearSSZ11,
		eLocationClearSSZ12,
		eLocationClearSSZ13,
		eLocationClearSSZ14,
		eLocationClearSSZ15,
		eLocationClearSSZ21,
		eLocationClearSSZ22,
		eLocationClearSSZ23,
		eLocationClearSSZ24,
		eLocationClearSSZ25,

		eLocationClearSPH11,
		eLocationClearSPH12,
		eLocationClearSPH13,
		eLocationClearSPH14,
		eLocationClearSPH15,
		eLocationClearSPH21,
		eLocationClearSPH22,
		eLocationClearSPH23,
		eLocationClearSPH24,
		eLocationClearSPH25,

		eLocationClearCTE11,
		eLocationClearCTE12,
		eLocationClearCTE13,
		eLocationClearCTE14,
		eLocationClearCTE15,
		eLocationClearCTE21,
		eLocationClearCTE22,
		eLocationClearCTE23,
		eLocationClearCTE24,
		eLocationClearCTE25,

		eLocationClearSSH11,
		eLocationClearSSH12,
		eLocationClearSSH13,
		eLocationClearSSH14,
		eLocationClearSSH15,
		eLocationClearSSH21,
		eLocationClearSSH22,
		eLocationClearSSH23,
		eLocationClearSSH24,
		eLocationClearSSH25,

		eLocationClearCSC11,
		eLocationClearCSC12,
		eLocationClearCSC13,
		eLocationClearCSC14,
		eLocationClearCSC15,
		eLocationClearCSC21,
		eLocationClearCSC22,
		eLocationClearCSC23,
		eLocationClearCSC24,
		eLocationClearCSC25,

		eLocationClearEUC11,
		eLocationClearEUC12,
		eLocationClearEUC13,
		eLocationClearEUC14,
		eLocationClearEUC15,
		eLocationClearEUC21,
		eLocationClearEUC22,
		eLocationClearEUC23,
		eLocationClearEUC24,
		eLocationClearEUC25,

		eLocationClearPLA11,
		eLocationClearPLA12,
		eLocationClearPLA13,
		eLocationClearPLA14,
		eLocationClearPLA15,
		eLocationClearPLA21,
		eLocationClearPLA22,
		eLocationClearPLA23,
		eLocationClearPLA24,
		eLocationClearPLA25,

		eLocationClearBMS1,
		eLocationClearBSD1,
		eLocationClearBSL1,
		eLocationClearBDE1,
		eLocationClearBPC1,
		eLocationClearBNE1,
		eLocationClearBLB1,

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

	struct GameSettings
	{
		bool HardBosses;
		bool Challenges;
	};
	
	inline bool LocationsChecked[ELocation::nLocation];
	inline bool ItemsCollected[EItem::nItem];

	void SetHardBosses(int enabled);
	void SetChallenges(int enabled);

	bool __fastcall IsLocationChecked(int id);
	bool TryCheckLocation(int id);
	bool __fastcall IsItemRecieved(int id);
	void ClearData();
	void OnItemRecieved(int id, bool alert);
	void OnLocationChecked(int id);

	bool InitAP();

	inline bool HasTriedInit;

	inline GameSettings pGameSettings;
}
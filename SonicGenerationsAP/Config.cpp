#include "Config.h"

bool CConfiguration::LoadConfig()
{
	if (ConfigLoaded)
		return true;

	INIReader reader(ConfigPath);

	if (reader.ParseError() != 0)
	{
		printf("INI read fail: %d", reader.ParseError());
		MessageBox(NULL, L"Failed to load Config.ini", NULL, MB_ICONERROR);
		return false;
	}

	APAddress = reader.Get("Connection", "ServerAddress", "localhost:38281");
	APPassword = reader.Get("Connection", "ServerPassword", "");
	APSlotName = reader.Get("Player", "SlotName", "SonicGenerations");

	ConfigLoaded = true;

	return true;
}

std::string CConfiguration::GetAPAddress()
{
	return APAddress;
}

std::string CConfiguration::GetAPSlotName()
{
	return APSlotName;
}

std::string CConfiguration::GetAPPassword()
{
	return APPassword;
}
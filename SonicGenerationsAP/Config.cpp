#include "Config.h"

bool Config::LoadConfig()
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

std::string Config::GetAPAddress()
{
	return APAddress;
}

std::string Config::GetAPSlotName()
{
	return APSlotName;
}

std::string Config::GetAPPassword()
{
	return APPassword;
}
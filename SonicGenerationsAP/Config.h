#pragma once

namespace Config
{
	inline bool ConfigLoaded;
	inline std::string APAddress;
	inline std::string APSlotName;
	inline std::string APPassword;

	inline std::string ConfigPath;

	std::string GetAPAddress();
	std::string GetAPSlotName();
	std::string GetAPPassword();

	bool LoadConfig();
};
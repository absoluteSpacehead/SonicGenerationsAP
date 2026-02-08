#pragma once

class CConfiguration
{
private:
	inline static bool ConfigLoaded;
	inline static std::string APAddress;
	inline static std::string APSlotName;
	inline static std::string APPassword;

public:
	inline static std::string ConfigPath;

	static std::string GetAPAddress();
	static std::string GetAPSlotName();
	static std::string GetAPPassword();

	static bool LoadConfig();
};
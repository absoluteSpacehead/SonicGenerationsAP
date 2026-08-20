#include "Utils.h"

std::string Utils::Trim(std::string string, const char character)
{
	string.erase(string.find_last_not_of(character) + 1);
	string.erase(0, string.find_first_not_of(character));

	return string;
}
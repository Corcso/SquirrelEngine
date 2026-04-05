#include "PCH.h"
#include "SQUtility.h"

#include <filesystem>

namespace SQ {
	std::vector<std::string> SplitString(std::string string, char delimiter) {
		std::vector<std::string> result;
		std::string currentString = "";
		// Loop over each character, adding to the current string if not the delimiter, and adding vector and clearing current string if it is. 
		for (int c = 0; c < string.length(); ++c) {
			if (string[c] != delimiter) currentString += string[c];
			else {
				result.push_back(currentString);
				currentString = "";
			}
		}
		result.push_back(currentString);
		return result;
	}
	std::string JoinString(const std::vector<std::string>& stringsToJoin, char delimiter)
	{
		std::string toReturn = "";

		// Join vector by the delimiter
		for (int s = 0; s < stringsToJoin.size(); ++s) {
			toReturn += stringsToJoin[s];
			if (s < stringsToJoin.size() - 1)toReturn += delimiter;
		}

		return toReturn;
	}

	std::wstring NStringToWString(const std::string& stringIn)
	{

		std::wstring toReturn = L"";

		// Loop over string
		for (int c = 0; c < stringIn.length(); ++c) {
			wchar_t wc = stringIn[c];
			toReturn += wc;
		}

		return toReturn;
	}
	std::string ConvertFullPathToRelative(std::string fullPath)
	{
		std::string workingDirectory = std::filesystem::current_path().string();
		std::replace(workingDirectory.begin(), workingDirectory.end(), '\\', '/');
		std::replace(fullPath.begin(), fullPath.end(), '\\', '/');
		std::string newString = ".";
		newString.reserve(fullPath.length());
		bool workingDirectoryPassed = false;
		for (int c = 0; c < fullPath.length(); c++) {
			if (workingDirectory.length() <= c || workingDirectory[c] != fullPath[c]) workingDirectoryPassed = true;

			if (workingDirectoryPassed) {
				newString += fullPath[c];
			}
		}

		return newString;
	}
}
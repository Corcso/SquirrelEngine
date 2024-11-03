#pragma once
#include "PCH.h"



namespace SQ {
	/// <summary>
	/// Splits a string based on the delimiter
	/// </summary>
	/// <param name="string">String to split</param>
	/// <param name="delimiter">Delimiter to split by</param>
	/// <returns>Vector of split strings</returns>
	std::vector<std::string> SplitString(std::string string, char delimiter);


	std::string JoinString(const std::vector<std::string>& stringsToJoin, char delimiter);
}


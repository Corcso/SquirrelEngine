#include "PCH.h"
#include "Utility.h"

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

		return result;
	}
}
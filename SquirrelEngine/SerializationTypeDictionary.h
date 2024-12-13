#pragma once
#include "PCH.h"
#include <functional>
#include <unordered_map>
#include "json.hpp"
#include "Nut.h"

namespace SQ {
	extern std::unordered_map<std::string, std::function<SQ::UniquePoolPtr<Nut>(Nut*, nlohmann::json)>> SerializationTypeDictionary;
}
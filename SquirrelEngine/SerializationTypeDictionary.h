#pragma once
#include "PCH.h"
#include <functional>
#include <unordered_map>
#include "json.hpp"
#include "Nut.h"

namespace SQ {
	/// <summary>
	/// <para>The Serialization Type Dictionary, used to deserialize .nut files and create the appropriate nut type for the text type. </para>
	/// <para>Defined here, initialised in Game. </para>
	/// </summary>
	extern std::unordered_map<std::string, std::function<SQ::UniquePoolPtr<Nut>(Nut*, nlohmann::json)>> SerializationTypeDictionary;
}
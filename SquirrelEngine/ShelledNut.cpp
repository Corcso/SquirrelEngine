#include "PCH.h"
#include "ShelledNut.h"
#include "SerializationTypeDictionary.h"
#include <fstream>

namespace SQ {
	ShelledNut* SQ::ShelledNut::Load(std::string path)
	{
		// Create a new shelled nut resource
		ShelledNut* newShelledNut = new ShelledNut();

		// Load File into json data then close file
		std::ifstream file(path, std::istream::in);
		if (!file.is_open()) return nullptr;
		
		newShelledNut->jsonData = nlohmann::json::parse(file);

		file.close();

		// Instantiation (The long part) Happens on developer demand, so end here. 
		return newShelledNut;
	}
	std::unique_ptr<Nut> ShelledNut::Instantiate()
	{
		return Instantiate(jsonData);
	}

	std::unique_ptr<Nut> ShelledNut::Instantiate(nlohmann::json data)
	{
		// Get Type
		std::string nutType = data["type"];

		// Create and deserialise this nut, we currently own this. 
		std::unique_ptr<Nut> newNut(SerializationTypeDictionary[nutType](nullptr, data));

		// Create and reparent all the children recursivly. 
		for (int i = 0; i < data["children"].size(); ++i) {
			// Create the child, ownership is ours
			std::unique_ptr<Nut> child = Instantiate(data["children"][i]);
			// Reparent the child, ownership is transfered to the parent
			child->SetParent(newNut.get());
			// We need to release our ownership.
			child.release();
		}

		// Return, moving ownership to the callee.
		return std::move(newNut);
	}
}
#include "PCH.h"
#include "ShelledNut.h"
#include "SerializationTypeDictionary.h"
#include "Services.h"
#include <fstream>

#include <thread>

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
	UniquePoolPtr<Nut> ShelledNut::Instantiate()
	{
		return Instantiate(jsonData);
	}

	std::shared_ptr<ShelledNut::InstantiatePromise> ShelledNut::InstantiateMultithread()
	{
		std::shared_ptr<InstantiatePromise> myRecordOfPromise(new InstantiatePromise());
		myRecordOfPromise->complete = false;

		std::thread worker(ShelledNut::InstantiateMultithreadWorkFunction, jsonData, myRecordOfPromise);
		worker.detach();

		return myRecordOfPromise;
	}

	void ShelledNut::InstantiateMultithreadWorkFunction(nlohmann::json data, std::shared_ptr<ShelledNut::InstantiatePromise> promiseToActOn)
	{

		promiseToActOn->result = Instantiate(data);
		promiseToActOn->complete = true;

		return;
	}

	UniquePoolPtr<Nut> ShelledNut::Instantiate(nlohmann::json data)
	{
		// Get Type
		if (data["type"].is_null()) throw 16;
		std::string nutType = data["type"];

		// Create and deserialise this nut, we currently own this. 
		UniquePoolPtr<Nut> newNut = SerializationTypeDictionary[nutType](nullptr, data);

		// Create and reparent all the children recursivly. 
		if (!data["children"].is_null()) {
			for (int i = 0; i < data["children"].size(); ++i) {
				// Create the child, ownership is ours
				UniquePoolPtr<Nut> child = Instantiate(data["children"][i]);
				Nut* temp = child.get();
				// Reparent the child, ownership is transfered to the parent
				temp->SetParent(newNut.get(), std::move(child));
				// We need to release our ownership.
				//child.release();
			}
		}

		// Return, moving ownership to the callee.
		return std::move(newNut);
	}
}
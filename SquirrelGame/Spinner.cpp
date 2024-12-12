#include "Spinner.h"

Spinner::Spinner()
{
	toLoad = SQ::Services::GetResourceManager()->Retrieve<SQ::ShelledNut>("./Resources/largeModel.nut");
}

SQ::Nut* Spinner::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
	// Cast deserializeInto to our type, call it toWorkOn
	Spinner* toWorkOn = dynamic_cast<Spinner*>(deserializeInto);
	// If toWorkOn is nullptr, make a new nut of our type. 
	if (toWorkOn == nullptr) toWorkOn = new Spinner();
	// Call parent deserialise, passing in our toWorkOn.
	SQ::MeshNut::Deserialize(toWorkOn, serializedData);

	// Perform deserialization on our data. 

	// Return toWorkOn
	return toWorkOn;
}

void Spinner::Update()
{
	RotateLocalY(0.0001f);

	if (SQ::Services::GetInput()->IsKeyPressed('F')) { 
		promise = toLoad->InstantiateMultithread(); 
		//a = SQ::Services::GetResourceManager()->Retrieve<SQ::Mesh>("./Resources/HQsphere.obj");
	}
	
	if (promise.get() != nullptr && promise->complete && !loaded) {
		AddChild(std::move(promise->result));
		loaded = true;
	}
}

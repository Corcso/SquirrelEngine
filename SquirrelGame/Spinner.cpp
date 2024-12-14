#include "Spinner.h"
#include <iostream>
Spinner::Spinner()
{
	toLoad = SQ::Services::GetResourceManager()->Retrieve<SQ::ShelledNut>("./Resources/crazyNumber.nut");
}

SQ::UniquePoolPtr<SQ::Nut> Spinner::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
	// Cast deserializeInto to our type, call it toWorkOn
	Spinner* toWorkOn = dynamic_cast<Spinner*>(deserializeInto);
	// If toWorkOn is nullptr, make a new nut of our type. 
	SQ::UniquePoolPtr<Nut> owner;
	if (deserializeInto == nullptr) {
		SQ::UniquePoolPtr<Spinner> instance = SQ::Services::GetPoolAllocationService()->MakeUniquePoolPtr<Spinner>();
		toWorkOn = instance.get();
		owner = instance.StaticUniquePoolPtrCast<Nut>();
		deserializeInto = owner.get();
	}
	// Call parent deserialise, passing in our toWorkOn.
	SQ::MeshNut::Deserialize(toWorkOn, serializedData);

	// Perform deserialization on our data. 

	// Return toWorkOn
	return owner;
}

void Spinner::Update()
{
	float rotate = 3.14 * SQ::GetTime()->Delta();

	RotateLocalY(rotate);

	float ypos = sin(SQ::GetTime()->TimeSinceStart());

	SetPosition(SQ::V3(0, ypos, 0));

	if (SQ::GetInput()->IsKeyPressed('F')) { 
		promise = toLoad->InstantiateMultithread(); 
		//promise = std::shared_ptr<SQ::ShelledNut::InstantiatePromise>(nullptr);
		//a = SQ::Services::GetResourceManager()->Retrieve<SQ::Mesh>("./Resources/HQsphere.obj");
	}

	if (SQ::GetInput()->IsKeyPressed('G')) {
		std::cout << SQ::GetPoolAllocationService()->GetDebugPoolUsageStatement();
	}

	if (SQ::GetInput()->IsKeyPressed('H')) {
		
		GetNut("LARGEMODEL")->QueueDestroy();
	}
	
	if (promise.get() != nullptr && promise->complete && !loaded) {
		AddChild(std::move(promise->result));
		loaded = true;
	}
}

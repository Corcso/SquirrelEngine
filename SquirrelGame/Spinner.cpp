#include "Spinner.h"

Spinner::Spinner()
{
	toLoad = SQ::Services::GetResourceManager()->Retrieve<SQ::ShelledNut>("./Resources/largeModel.nut");
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
	RotateLocalY(0.0001f);

	if (SQ::Services::GetInput()->IsKeyPressed('F')) { 
		promise = toLoad->InstantiateMultithread(); 
		//promise = std::shared_ptr<SQ::ShelledNut::InstantiatePromise>(nullptr);
		//a = SQ::Services::GetResourceManager()->Retrieve<SQ::Mesh>("./Resources/HQsphere.obj");
	}
	
	if (promise.get() != nullptr && promise->complete && !loaded) {
		AddChild(std::move(promise->result));
		loaded = true;
	}
}
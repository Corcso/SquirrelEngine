#include "PCH.h"
#include "MeshNut.h"
#include "Services.h"
namespace SQ {
	UniquePoolPtr<Nut> MeshNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
	{
		// Cast deserializeInto to our type, call it toWorkOn
		MeshNut* toWorkOn = dynamic_cast<MeshNut*>(deserializeInto);
		// If toWorkOn is nullptr, make a new nut of our type. 
		// We need to follow strict ownership with the pool ptr
		UniquePoolPtr<Nut> owner;
		if (toWorkOn == nullptr) {
			// Get the instance
			UniquePoolPtr<MeshNut> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<MeshNut>();
			// Set to work on to the instance
			toWorkOn = instance.get();
			// Transfer ownership into owner and static cast to nut base class
			owner = instance.StaticUniquePoolPtrCast<Nut>();
		}
		// Call parent deserialise, passing in our toWorkOn.
		WorldNut::Deserialize(toWorkOn, serializedData);

		// Perform deserialization on our data. 
		if (!serializedData["mesh"].is_null()) toWorkOn->SetMesh(Services::GetResourceManager()->Retrieve<Mesh>(serializedData["mesh"]));
		if (!serializedData["material"].is_null()) toWorkOn->SetMaterial(Services::GetResourceManager()->Retrieve<Material>(serializedData["material"]));

		// Return toWorkOn
		return owner;
	}

	void SQ::MeshNut::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		this->mesh = mesh;
	}

	std::shared_ptr<Mesh> MeshNut::GetMesh()
	{
		return mesh;
	}
	void MeshNut::SetMaterial(std::shared_ptr<Material> material)
	{
		this->material = material;
	}
	std::shared_ptr<Material> MeshNut::GetMaterial()
	{
		return material;
	}
}
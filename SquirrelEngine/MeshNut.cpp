#include "PCH.h"
#include "MeshNut.h"
#include "Services.h"
namespace SQ {
	UniquePoolPtr<Nut> MeshNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
	{
		// Cast deserializeInto to our type, call it toWorkOn
		MeshNut* toWorkOn = dynamic_cast<MeshNut*>(deserializeInto);
		// If toWorkOn is nullptr, make a new nut of our type. 
		UniquePoolPtr<Nut> owner;
		if (deserializeInto == nullptr) {
			UniquePoolPtr<MeshNut> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<MeshNut>();
			toWorkOn = instance.get();
			owner = instance.StaticUniquePoolPtrCast<Nut>();
			deserializeInto = owner.get();
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
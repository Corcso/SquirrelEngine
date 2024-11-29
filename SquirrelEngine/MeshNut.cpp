#include "PCH.h"
#include "MeshNut.h"
#include "Services.h"
namespace SQ {
	Nut* MeshNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
	{
		// Cast deserializeInto to our type, call it toWorkOn
		MeshNut* toWorkOn = dynamic_cast<MeshNut*>(deserializeInto);
		// If toWorkOn is nullptr, make a new nut of our type. 
		if (toWorkOn == nullptr) toWorkOn = new MeshNut();
		// Call parent deserialise, passing in our toWorkOn.
		WorldNut::Deserialize(toWorkOn, serializedData);

		// Perform deserialization on our data. 
		if (!serializedData["mesh"].is_null()) toWorkOn->SetMesh(Services::GetResourceManager()->Retrieve<Mesh>(serializedData["mesh"]));

		// Return toWorkOn
		return toWorkOn;
	}

	void SQ::MeshNut::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		this->mesh = mesh;
	}

	std::shared_ptr<Mesh> MeshNut::GetMesh()
	{
		return mesh;
	}
}
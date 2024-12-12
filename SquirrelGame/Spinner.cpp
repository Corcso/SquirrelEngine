#include "Spinner.h"

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
}

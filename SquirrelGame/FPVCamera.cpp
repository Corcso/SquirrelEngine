#include "FPVCamera.h"
#include "Services.h"

Nut* FPVCamera::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
	// Cast deserializeInto to our type, call it toWorkOn
	FPVCamera* toWorkOn = dynamic_cast<FPVCamera*>(deserializeInto);
	// If toWorkOn is nullptr, make a new nut of our type. 
	if (toWorkOn == nullptr) toWorkOn = new FPVCamera();
	// Call parent deserialise, passing in our toWorkOn.
	CameraNut::Deserialize(toWorkOn, serializedData);

	// Perform deserialization on our data. 

	// Return toWorkOn
	return toWorkOn;
}

void FPVCamera::Update()
{
	if (SQ::Services::GetInput()->IsKeyDown('q')) SQ::Services::GetInput()->LockMouse();
	else if (SQ::Services::GetInput()->IsKeyDown('e')) SQ::Services::GetInput()->UnlockMouse();

	if (Services::GetInput()->IsKeyDown('A')) SetPosition(GetPosition() - GetRight() * 0.0005f);
	if (Services::GetInput()->IsKeyDown('D')) SetPosition(GetPosition() + GetRight() * 0.0005f);
	if (Services::GetInput()->IsKeyDown('W')) SetPosition(GetPosition() + GetForward() * 0.0005f);
	if (Services::GetInput()->IsKeyDown('S')) SetPosition(GetPosition() - GetForward() * 0.0005f);

	float sens = -0.005f;

	if (SQ::Services::GetInput()->IsMouseLocked()) { 
		//myCam.SetEulerAngles(SQ::V3(myCam.GetEulerAngles().X + (SQ::Services::GetInput()->GetMouseMovement().Y * sens), myCam.GetEulerAngles().Y + (SQ::Services::GetInput()->GetMouseMovement().X * sens), myCam.GetEulerAngles().Z)); 
		RotateGlobalY(SQ::Services::GetInput()->GetMouseMovement().X * sens);
		RotateLocalX(SQ::Services::GetInput()->GetMouseMovement().Y * sens);	
		//std::cout << "Ri: " << myCam.GetRight().X << " " << myCam.GetRight().Y << " " << myCam.GetRight().Z << "\n";
	}
}

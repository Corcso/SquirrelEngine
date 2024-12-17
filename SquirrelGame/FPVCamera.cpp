#include "FPVCamera.h"
#include "Services.h"

#include <iostream>

UniquePoolPtr<Nut> FPVCamera::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // REQUIRED DESERIALIZATION CODE BLOCK, MAKE TOWORKON & INSTANCE TO YOUR NUT TYPE
    // Cast deserializeInto to our type, call it toWorkOn
    FPVCamera* toWorkOn = dynamic_cast<FPVCamera*>(deserializeInto);
    // If toWorkOn is nullptr, make a new nut of our type. 
    // We need to follow strict ownership with the pool ptr
    UniquePoolPtr<Nut> owner;
    if (toWorkOn == nullptr) {
        // Get the instance
        UniquePoolPtr<FPVCamera> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<FPVCamera>();
        // Set to work on to the instance
        toWorkOn = instance.get();
        // Transfer ownership into owner and static cast to nut base class
        owner = instance.StaticUniquePoolPtrCast<Nut>();
    }
    // REQUIRED PARENT DESERIALIZATION, CALL PARENT CLASS
    // Call parent deserialise, passing in our toWorkOn.
    CameraNut::Deserialize(toWorkOn, serializedData);

    // Perform deserialization on our data. 

    // YOUR DESERIALIZATION HERE, USE TOWORKON

    // Return ownership of the new nut
    return owner;
}

void FPVCamera::Ready()
{
	CameraNut::Ready();
}

void FPVCamera::Update()
{
	CameraNut::Update();

	// Lock mouse with Q and E
	if (SQ::Services::GetInput()->IsKeyPressed('q')) SQ::Services::GetInput()->LockMouse();
	else if (SQ::Services::GetInput()->IsKeyReleased('e')) SQ::Services::GetInput()->UnlockMouse();

	// Move with WASD
	if (Services::GetInput()->IsKeyDown('A')) SetPosition(GetPosition() - (GetLocalRight() * 3.0f * GetTime()->Delta()));
	if (Services::GetInput()->IsKeyDown('D')) SetPosition(GetPosition() + (GetLocalRight() * 3.0f * GetTime()->Delta()));
	if (Services::GetInput()->IsKeyDown('W')) SetPosition(GetPosition() + (GetLocalForward() * 3.0f * GetTime()->Delta()));
	if (Services::GetInput()->IsKeyDown('S')) SetPosition(GetPosition() - (GetLocalForward() * 3.0f * GetTime()->Delta()));

	// Sensitivity
	float sens = -0.005f;

	if (SQ::Services::GetInput()->IsMouseLocked()) { 
		// Rotate on the global Y (0, 1, 0)
		RotateGlobalY(SQ::Services::GetInput()->GetMouseMovement().X * sens);
		// And the super local X, the right vector but only with this nut's (camera) rotations applied. 
		RotateSuperLocalX(SQ::Services::GetInput()->GetMouseMovement().Y * sens);	
	}
}

void FPVCamera::LateUpdate()
{
	CameraNut::LateUpdate();
}

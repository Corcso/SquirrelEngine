#include "PCH.h"
#include "EditorCameraNut.h"
#include "Services.h"

namespace SQ {

	void SQ::EditorCameraNut::Ready()
	{
		CameraNut::Ready();
	}

	void SQ::EditorCameraNut::Update()
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

	void SQ::EditorCameraNut::LateUpdate()
	{
		CameraNut::LateUpdate();
	}
}
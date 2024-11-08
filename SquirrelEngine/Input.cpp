#include "PCH.h"
#include "Input.h"
namespace SQ {
	void Input::SetKeyState(char key, InputState state)
	{
		keys[key] = state;
	}

	void Input::SetMouseState(MouseButton button, InputState state)
	{
		mouseButtons[static_cast<int>(button)] = state;
	}

	bool Input::IsKeyUp(char key)
	{
		return (static_cast<int>(keys[key]) & (static_cast<int>(InputState::UP) | static_cast<int>(InputState::RELEASED)));
	}
	bool Input::IsKeyReleased(char key)
	{
		return (static_cast<int>(keys[key]) & static_cast<int>(InputState::RELEASED));
	}

	bool Input::IsKeyDown(char key)
	{
		return (static_cast<int>(keys[key]) & (static_cast<int>(InputState::DOWN) | static_cast<int>(InputState::PRESSED)));
	}
	bool Input::IsKeyPressed(char key)
	{
		return (static_cast<int>(keys[key]) & static_cast<int>(InputState::PRESSED));
	}
}
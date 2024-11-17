#include "PCH.h"
#include "Input.h"
namespace SQ {
	bool Input::IsMouseLocked()
	{
		return isMouseLocked;
	}
	void Input::SetKeyState(Key key, InputState state)
	{
		keys[static_cast<int>(key)] = state;
	}

	void Input::SetMouseState(MouseButton button, InputState state)
	{
		mouseButtons[static_cast<int>(button)] = state;
	}

	void Input::SetMousePosition(Vec2 newMousePosition)
	{
		mousePositionThisFrame = newMousePosition;
	}

	void Input::SetMouseMovement(Vec2 mouseMovement)
	{
		this->mouseMovement = mouseMovement;
	}

	Input::Input()
	{
		// Setup character lookup table
		// Clear all values
		for (int c = 0; c < 123; ++c) {
			characterLookupTable[c] = Key::INVALID_KEY;
		}
		// 0->31 ascii values unused
		// Value 32 is space
		characterLookupTable[32] = Key::SPACE;
		// Values 33->47 are unused
		// Values 48->57 are One to nine
		for (int c = 48; c < 58; ++c) {
			characterLookupTable[c] = static_cast<Key>(static_cast<int>(Key::ONE) + (c - 48));
		}
		// Values 58->64 are unused
		// Values 65->90 are uppercase A to Z
		for (int c = 65; c < 91; ++c) {
			characterLookupTable[c] = static_cast<Key>(static_cast<int>(Key::A) + (c - 65));
		}
		// Values 91->96 are unused
		// Values 97->123 are lowercase A to Z
		for (int c = 97; c < 123; ++c) {
			characterLookupTable[c] = static_cast<Key>(static_cast<int>(Key::A) + (c - 97));
		}
		// Any values after are not used 

		// Set all keys to start up
		for (int k = 0; k < static_cast<int>(Key::TOTAL_SUPPORTED_KEYS); ++k) {
			keys[k] = InputState::UP;
		}
	}

	void Input::Update()
	{
		// Set all pressed keys to down, all released keys to up and all invalid keys to up
		for (int k = 0; k < static_cast<int>(Key::TOTAL_SUPPORTED_KEYS); ++k) {
			if (keys[k] == InputState::PRESSED) keys[k] = InputState::DOWN;
			else if (keys[k] == InputState::RELEASED) keys[k] = InputState::UP;
			else if (keys[k] == InputState::INVALID) keys[k] = InputState::UP;
		}

		// Set all pressed buttons to down, all released buttons to up and all invalid buttons to up
		for (int b = 0; b < static_cast<int>(MouseButton::TOTAL_SUPPORTED_BUTTONS); ++b) {
			if (mouseButtons[b] == InputState::PRESSED) mouseButtons[b] = InputState::DOWN;
			else if (mouseButtons[b] == InputState::RELEASED) mouseButtons[b] = InputState::UP;
			else if (mouseButtons[b] == InputState::INVALID) mouseButtons[b] = InputState::UP;
		}

		// Set mouse position this frame to mouse position last frame
		mousePositionLastFrame = mousePositionThisFrame;

		return;
	}

	bool Input::IsKeyUp(Key key)
	{
		return (static_cast<int>(keys[static_cast<int>(key)]) & (static_cast<int>(InputState::UP) | static_cast<int>(InputState::RELEASED)));
	}
	bool Input::IsKeyReleased(Key key)
	{
		return (static_cast<int>(keys[static_cast<int>(key)]) & static_cast<int>(InputState::RELEASED));
	}

	bool Input::IsKeyDown(Key key)
	{
		return (static_cast<int>(keys[static_cast<int>(key)]) & (static_cast<int>(InputState::DOWN) | static_cast<int>(InputState::PRESSED)));
	}
	bool Input::IsKeyPressed(Key key)
	{
		return (static_cast<int>(keys[static_cast<int>(key)]) & static_cast<int>(InputState::PRESSED));
	}
	bool Input::IsKeyUp(char key)
	{
		return IsKeyUp(characterLookupTable[key]);
	}
	bool Input::IsKeyReleased(char key)
	{
		return IsKeyReleased(characterLookupTable[key]);
	}
	bool Input::IsKeyDown(char key)
	{
		return IsKeyDown(characterLookupTable[key]);
	}
	bool Input::IsKeyPressed(char key)
	{
		return IsKeyPressed(characterLookupTable[key]);
	}
	bool Input::IsMouseUp(MouseButton button)
	{
		return (static_cast<int>(mouseButtons[static_cast<int>(button)]) & (static_cast<int>(InputState::UP) | static_cast<int>(InputState::RELEASED)));
	}
	bool Input::IsMouseReleased(MouseButton button)
	{
		return (static_cast<int>(mouseButtons[static_cast<int>(button)]) & static_cast<int>(InputState::RELEASED));
	}
	bool Input::IsMouseDown(MouseButton button)
	{
		return (static_cast<int>(mouseButtons[static_cast<int>(button)]) & (static_cast<int>(InputState::DOWN) | static_cast<int>(InputState::PRESSED)));
	}
	bool Input::IsMousePressed(MouseButton button)
	{
		return (static_cast<int>(mouseButtons[static_cast<int>(button)]) & static_cast<int>(InputState::PRESSED));
	}
	Vec2 Input::GetMousePosition()
	{
		return mousePositionThisFrame;
	}
	Vec2 Input::GetMousePositionLastFrame()
	{
		return mousePositionLastFrame;
	}
	Vec2 Input::GetMouseMovement()
	{
		return mouseMovement;
	}
}
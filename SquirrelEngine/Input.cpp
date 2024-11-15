#include "PCH.h"
#include "Input.h"
namespace SQ {
	void Input::SetKeyState(Key key, InputState state)
	{
		keys[static_cast<int>(key)] = state;
	}

	void Input::SetMouseState(MouseButton button, InputState state)
	{
		mouseButtons[static_cast<int>(button)] = state;
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
}
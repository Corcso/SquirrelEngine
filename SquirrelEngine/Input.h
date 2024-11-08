#pragma once
namespace SQ {
	class Input
	{
	public:
		virtual void Update() = 0;

		enum class InputState {
			INVALID = 0x0,
			UP = 0x1,
			RELEASED = 0x2,
			DOWN = 0x4,
			PRESSED = 0x8
		};

		bool IsKeyUp(char key);
		bool IsKeyReleased(char key);
		bool IsKeyDown(char key);
		bool IsKeyPressed(char key);

		enum class MouseButton { LEFT, RIGHT, BUTTON_COUNT };

		bool IsMouseUp(MouseButton button);
		bool IsMouseReleased(MouseButton button);
		bool IsMouseDown(MouseButton button);
		bool IsMousePressed(MouseButton button);

		void SetKeyState(char key, InputState state);
		void SetMouseState(MouseButton button, InputState state);
	protected:
		InputState keys[256];
		InputState mouseButtons[static_cast<int>(MouseButton::BUTTON_COUNT)];
	};
}
#pragma once
namespace SQ {
	class Input
	{
	public:
		virtual void Update() = 0;

		bool IsKeyUp(char key);
		bool IsKeyReleased(char key);
		bool IsKeyDown(char key);
		bool IsKeyPressed(char key);

		enum class MouseButton { LEFT, RIGHT, BUTTON_COUNT };

		bool IsMouseUp(MouseButton key);
		bool IsMouseReleased(MouseButton key);
		bool IsMouseDown(MouseButton key);
		bool IsMousePressed(MouseButton key);

	protected:
		enum class InputState {
			INVALID = 0x0,
			UP = 0x1,
			RELEASED = 0x2,
			DOWN = 0x4,
			PRESSED = 0x8
		};

		InputState Keys[256];
		InputState MouseButtons[static_cast<int>(MouseButton::BUTTON_COUNT)];
	};
}
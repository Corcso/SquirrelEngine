#pragma once
#include "Math.h"

namespace SQ {
	class Input
	{
	public:
		Input();

		enum class InputState {
			INVALID = 0x0,
			UP = 0x1,
			RELEASED = 0x2,
			DOWN = 0x4,
			PRESSED = 0x8
		};

		enum class Key {
			INVALID_KEY,
			BACKSPACE,
			TAB,
			ENTER,
			SHIFT,
			CONTROL,
			ALT, // todo support r AND l 
			PAUSE,
			CAPSLOCK,
			ESCAPE,
			SPACE,
			PAGE_UP,
			PAGE_DOWN,
			HOME,
			END,
			INSERT,
			DEL,
			LEFT_ARROW,
			RIGHT_ARROW,
			UP_ARROW,
			DOWN_ARROW,
			PRINT,
			ZERO,
			ONE,
			TWO,
			THREE,
			FOUR,
			FIVE,
			SIX,
			SEVEN,
			EIGHT,
			NINE,
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			// todo add oem

			TOTAL_SUPPORTED_KEYS
		};

		Key characterLookupTable[123];

		virtual void ProcessInput() = 0;
		virtual void Update();

		bool IsKeyUp(Key key);
		bool IsKeyReleased(Key key);
		bool IsKeyDown(Key key);
		bool IsKeyPressed(Key key);

		bool IsKeyUp(char key);
		bool IsKeyReleased(char key);
		bool IsKeyDown(char key);
		bool IsKeyPressed(char key);

		enum class MouseButton { LEFT, RIGHT, TOTAL_SUPPORTED_BUTTONS };

		bool IsMouseUp(MouseButton button);
		bool IsMouseReleased(MouseButton button);
		bool IsMouseDown(MouseButton button);
		bool IsMousePressed(MouseButton button);

		Vec2 GetMousePosition();
		Vec2 GetMousePositionLastFrame();
		Vec2 GetMouseMovement();

		virtual void LockMouse() = 0;
		virtual void UnlockMouse() = 0;
		bool IsMouseLocked();

		void SetKeyState(Key key, InputState state);
		void SetMouseState(MouseButton button, InputState state);
		void SetMousePosition(Vec2 newMousePosition);
		void SetMouseMovement(Vec2 mouseMovement);

	protected:
		bool isMouseLocked;

		Vec2 mousePositionLastFrame;
		Vec2 mousePositionThisFrame;
		Vec2 mouseMovement;

		InputState keys[static_cast<int>(Key::TOTAL_SUPPORTED_KEYS)];
		InputState mouseButtons[static_cast<int>(MouseButton::TOTAL_SUPPORTED_BUTTONS)];
	};
}
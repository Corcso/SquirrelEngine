#pragma once
#include "SQMath.h"

namespace SQ {
	class Input
	{
	public:
		Input();

		/// <summary>
		/// <para>Input state. </para>
		/// <para>Used for storing button or key states.</para>
		/// <para>If PRESSED is true, If DOWN should also be true.</para>
		/// <para>If RELEASED is true, If UP should also be true.</para>
		/// </summary>
		enum class InputState : unsigned char {
			INVALID = 0x0,
			UP = 0x1,
			RELEASED = 0x2,
			DOWN = 0x4,
			PRESSED = 0x8,
			SAME_FRAME_PRESS_RELEASE = 0xA // 0x2 | 0x8 (Pressed and released)
		};

		/// <summary>
		/// <para>Key Enum</para>
		/// <para>All keys which Squirrel Engine can interact with.</para>
		/// </summary>
		enum class Key {
			INVALID_KEY,
			BACKSPACE,
			TAB,
			ENTER,
			SHIFT,
			CONTROL,
			ALT, // Possible Addition: support right and left variants
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
			// Possible Addition: Add OEM Keys

			TOTAL_SUPPORTED_KEYS
		};

		/// <summary>
		/// <para>Character lookup table. </para>
		/// <para>At index ascii value, Key enum is stored.</para>
		/// </summary>
		Key characterLookupTable[123];

		/// <summary>
		/// Processes input, called before update as part of the game loop.
		/// </summary>
		virtual void ProcessInput() = 0;

		/// <summary>
		/// Updates the input class, this should be called at the end of the frame.
		/// </summary>
		virtual void Update();

		/// <summary>
		/// <para>Is key up.</para>
		/// <para>If a key is released, it is classed as up</para>
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>True if up/released, False if down/pressed</returns>
		bool IsKeyUp(Key key);
		
		/// <summary>
		/// <para>Is key released.</para>
		/// <para>A key is released only for the frame it is released on.</para>
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>True if released, false otherwise.</returns>
		bool IsKeyReleased(Key key);
		/// <summary>
		/// <para>Is key down.</para>
		/// <para>If a key is pressed, it is classed as down</para>
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>True if down/pressed, False if up/released</returns>
		bool IsKeyDown(Key key);
		
		/// <summary>
		/// <para>Is key pressed.</para>
		/// <para>A key is pressed only for the frame it is pressed on.</para>
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>True if pressed, false otherwise.</returns>
		bool IsKeyPressed(Key key);

		/// <summary>
		/// <para>Is key up.</para>
		/// <para>If a key is released, it is classed as up</para>
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>True if up/released, False if down/pressed</returns>
		bool IsKeyUp(char key);
		/// <summary>
		/// <para>Is key released.</para>
		/// <para>A key is released only for the frame it is released on.</para>
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>True if released, false otherwise.</returns>
		bool IsKeyReleased(char key);
		/// <summary>
		/// <para>Is key down.</para>
		/// <para>If a key is pressed, it is classed as down</para>
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>True if down/pressed, False if up/released</returns>
		bool IsKeyDown(char key);
		/// <summary>
		/// <para>Is key pressed.</para>
		/// <para>A key is pressed only for the frame it is pressed on.</para>
		/// </summary>
		/// <param name="key">Key to check</param>
		/// <returns>True if pressed, false otherwise.</returns>
		bool IsKeyPressed(char key);

		/// <summary>
		/// <para>Mouse Button</para>
		/// <para>All Mouse Buttons which squirrel engine supports</para>
		/// </summary>
		enum class MouseButton { LEFT, RIGHT, TOTAL_SUPPORTED_BUTTONS };

		/// <summary>
		/// <para>Is mouse button up.</para>
		/// <para>If a mouse button is released, it is classed as up</para>
		/// </summary>
		/// <param name="button">Mouse button to check</param>
		/// <returns>True if up/released, False if down/pressed</returns>
		bool IsMouseUp(MouseButton button);
		/// <summary>
		/// <para>Is mouse button released.</para>
		/// <para>A mouse button is released only for the frame it is released on.</para>
		/// </summary>
		/// <param name="button">Mouse button to check</param>
		/// <returns>True if released, false otherwise.</returns>
		bool IsMouseReleased(MouseButton button);
		/// <summary>
		/// <para>Is mouse button down.</para>
		/// <para>If a mouse button is pressed, it is classed as down</para>
		/// </summary>
		/// <param name="button">Mouse button to check</param>
		/// <returns>True if down/pressed, False if up/released</returns>
		bool IsMouseDown(MouseButton button);
		/// <summary>
		/// <para>Is mouse button pressed.</para>
		/// <para>A mouse button is pressed only for the frame it is pressed on.</para>
		/// </summary>
		/// <param name="button">Mouse button to check</param>
		/// <returns>True if pressed, false otherwise.</returns>
		bool IsMousePressed(MouseButton button);

		/// <summary>
		/// Returns the mouse position in pixel coordinates. 
		/// </summary>
		/// <returns>Mouse position in pixel coordinates</returns>
		Vec2 GetMousePosition();
		/// <summary>
		/// Returns the mouse position in pixel coordinates from the last frame.
		/// </summary>
		/// <returns>Last mouse position in pixel coordinates</returns>
		Vec2 GetMousePositionLastFrame();
		/// <summary>
		/// <para>Returns the distance the mouse has moved. </para>
		/// <para>If mouse lock is off, this is in pixels</para>
		/// <para>If mouse lock is on, numbers are typically smaller, around 0-4</para>
		/// </summary>
		/// <returns>How far the mouse has moved since last frame</returns>
		Vec2 GetMouseMovement();

		/// <summary>
		/// <para>Locks and hides the mouse</para>
		/// <para>Useful for first person views</para>
		/// </summary>
		virtual void LockMouse() = 0;
		/// <summary>
		/// <para>Unlocks and shows the mouse</para>
		/// <para>Useful for menus</para>
		/// </summary>
		virtual void UnlockMouse() = 0;

		/// <summary>
		/// Check if the mouse is locked
		/// </summary>
		/// <returns>True if locked, False if not.</returns>
		bool IsMouseLocked();

		/// <summary>
		/// <para>Set Key's State</para>
		/// <para>Be Careful! This is a function intended for engine use. You can use it, if you know what you are doing.</para>
		/// </summary>
		/// <param name="key">Key to change</param>
		/// <param name="state">State to set to</param>
		void SetKeyState(Key key, InputState state);

		/// <summary>
		/// <para>Set Mouse Button's State</para>
		/// <para>Be Careful! This is a function intended for engine use. You can use it, if you know what you are doing.</para>
		/// </summary>
		/// <param name="button">Mouse button to change</param>
		/// <param name="state">State to set to</param>
		void SetMouseState(MouseButton button, InputState state);

		/// <summary>
		/// <para>Set Mouse's Position</para>
		/// <para>Be Careful! This is a function intended for engine use. You can use it, if you know what you are doing.</para>
		/// </summary>
		/// <param name="newMousePosition">New mouse position, in pixels</param>
		void SetMousePosition(Vec2 newMousePosition);

		/// <summary>
		/// <para>Set Mouse's Position</para>
		/// <para>Be Careful! This is a function intended for engine use. You can use it, if you know what you are doing.</para>
		/// </summary>
		/// <param name="mouseMovement">New mouse movement</param>
		void SetMouseMovement(Vec2 mouseMovement);

		void ImGuiRenderDebugInfo();

	protected:
		bool isMouseLocked;

		Vec2 mousePositionLastFrame;
		Vec2 mousePositionThisFrame;
		Vec2 mouseMovement;

		InputState keys[static_cast<int>(Key::TOTAL_SUPPORTED_KEYS)];
		InputState mouseButtons[static_cast<int>(MouseButton::TOTAL_SUPPORTED_BUTTONS)];
	};
}
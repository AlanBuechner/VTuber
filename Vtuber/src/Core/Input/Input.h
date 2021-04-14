#pragma once

#include <queue>
#include <vector>

namespace Engine
{
	class Input
	{

	public:

		enum MouseCodes
		{
			LMouse = 0,
			RMouse = 1,
			MMouse = 2
		};

		enum class KeyState
		{
			Up = 0,
			Pressed = 1,
			Released = 2,
			Down = 3
		};

		class Event
		{
		public:
			enum class Type
			{
				Press,
				Release,
				Move,
				WheelUp,
				WheelDown,
				Invalid
			};

			Event();
			Event(Type type, unsigned char code);
			Event(Type type, int x, int y);
			~Event();

			inline Type GetType() { return type; }
			inline unsigned char GetCode() { return code; }

			inline int GetX() { return x; }
			inline int GetY() { return y; }

		private:
			Type type;
			unsigned char code;

			int x, y;

		};

	public:
		Input() = default;
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		~Input();

		// mouse input

		inline KeyState GetButtonState(unsigned char keycode) { return m_ButtonState[keycode]; }

		inline bool GetButtonUp(unsigned char keycode) { return GetButtonState(keycode) == KeyState::Up; }
		inline bool GetButtonPressed(unsigned char keycode) { return GetButtonState(keycode) == KeyState::Pressed; }
		inline bool GetButtonReleased(unsigned char keycode) { return GetButtonState(keycode) == KeyState::Released; }
		inline bool GetButtonDown(unsigned char keycode) { return GetButtonState(keycode) == KeyState::Down; }

		Event ReadButton();
		bool ButtonIsEmpty() const;
		void FlushButton();

		int GetMouseX() { return x; }
		int GetMouseY() { return y; }

		// keyboard input
		inline KeyState GetKeyState(unsigned char keycode) { return m_KeyState[keycode]; }

		inline bool GetKeyUp(unsigned char keycode) { return GetKeyState(keycode) == KeyState::Up; }
		inline bool GetKeyPressed(unsigned char keycode) { return GetKeyState(keycode) == KeyState::Pressed; }
		inline bool GetKeyReleased(unsigned char keycode) { return GetKeyState(keycode) == KeyState::Released; }
		inline bool GetKeyDown(unsigned char keycode) { return GetKeyState(keycode) == KeyState::Down; }

		Event ReadKey();
		bool KeyIsEmpty() const;
		void FlushKey();

		char ReadChar();
		bool CharIsEmpty() const;
		void FlushChar();
		void Flush();

		void EnableAutorepeat();
		void DisableAutorepeat();
		bool AutorepeatEnabled() const;

	private:
		// mouse
		void OnMouseMove(int x, int y);
		void OnMousePressed(unsigned char keycode);
		void OnMouseReleased(unsigned char keycode);

		// keyboard
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		void OnChar(char character);

		void UpdateKeyStates();
		void ClearKeyStates();

		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);

	private:
		static constexpr unsigned int bufferSize = 16u;
		// mouse input
		int x = 0, y = 0;
		KeyState* m_ButtonState = new KeyState[3u];
		std::queue<Event> m_MouseBuffer;
		std::queue<unsigned char> m_UpdatedButtons;

		// keyboard input
		static constexpr unsigned int nKeys = 256u;
		bool m_AutorepeatEnabled = false;
		KeyState* m_KeyState = new KeyState[nKeys];
		std::queue<unsigned char> m_UpdatedKeys;
		std::queue<Event> m_KeyBuffer;
		std::queue<char> m_CharBuffer;

		friend class Window;
		friend class WindowsWindow;
	};

	template<typename T>
	inline void Input::TrimBuffer(std::queue<T>& buffer)
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}
}
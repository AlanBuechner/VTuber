#include "Input.h"

namespace Engine
{

#pragma region Event

	Input::Event::Event() :
		type(Type::Invalid) ,
		code(0u), x(0), y(0)
	{}

	Input::Event::Event(Type type, unsigned char code) :
		type(type), code(code), x(0), y(0)
	{}

	Input::Event::Event(Type type, int x, int y) :
		type(type), code(0u), x(x), y(y)
	{}

	Input::Event::~Event()
	{
	}

#pragma endregion

	Input::~Input()
	{
		delete[] m_ButtonState;
		delete[] m_KeyState;
	}

	void Input::Flush()
	{
		FlushKey();
		FlushButton();
		FlushChar();
	}

#pragma region Mouse

	Input::Event Input::ReadButton()
	{
		if (m_KeyBuffer.size() > 0u)
		{
			Input::Event e = m_KeyBuffer.front();
			m_KeyBuffer.pop();
			return e;
		}
		else
		{
			return Input::Event();
		}
	}

	bool Input::ButtonIsEmpty() const
	{
		return m_KeyBuffer.empty();
	}

	void Input::FlushButton()
	{
		m_KeyBuffer = std::queue<Event>();
	}

	void Input::OnMouseMove(int newx, int newy)
	{
		x = newx; y = newy;

		m_MouseBuffer.push(Input::Event(Input::Event::Type::Move, newx, newy));
		TrimBuffer(m_MouseBuffer);
	}

	void Input::OnMousePressed(unsigned char keycode)
	{
		m_MouseBuffer.push(Input::Event(Input::Event::Type::Press, keycode));
		TrimBuffer(m_MouseBuffer);
	}

	void Input::OnMouseReleased(unsigned char keycode)
	{
		m_MouseBuffer.push(Input::Event(Input::Event::Type::Release, keycode));
		TrimBuffer(m_MouseBuffer);
	}


#pragma endregion

#pragma region Keyboard

	// read and set data

	Input::Event Input::ReadKey()
	{
		if (m_KeyBuffer.size() > 0u)
		{
			Input::Event e = m_KeyBuffer.front();
			m_KeyBuffer.pop();
			return e;
		}
		else
		{
			return Input::Event();
		}
	}

	bool Input::KeyIsEmpty() const
	{
		return m_KeyBuffer.empty();
	}

	void Input::FlushKey()
	{
		m_KeyBuffer = std::queue<Event>();
	}

	char Input::ReadChar()
	{
		if (m_CharBuffer.size() > 0u)
		{
			unsigned char charCode = m_CharBuffer.front();
			m_CharBuffer.pop();
			return charCode;
		}
		else
		{
			return 0;
		}
	}

	bool Input::CharIsEmpty() const
	{
		return m_CharBuffer.empty();
	}

	void Input::FlushChar()
	{
		m_CharBuffer = std::queue<char>();
	}


	void Input::EnableAutorepeat()
	{
		m_AutorepeatEnabled = true;
	}

	void Input::DisableAutorepeat()
	{
		m_AutorepeatEnabled = false;
	}

	bool Input::AutorepeatEnabled() const
	{
		return m_AutorepeatEnabled;
	}

	// on event handling

	void Input::OnKeyPressed(unsigned char keycode)
	{
		//m_KeyState[keycode] = KeyState::Pressed;
		m_KeyBuffer.push(Input::Event(Input::Event::Type::Press, keycode));
		TrimBuffer(m_KeyBuffer);
	}

	void Input::OnKeyReleased(unsigned char keycode)
	{
		//m_KeyState[keycode] = KeyState::Released;
		m_KeyBuffer.push(Input::Event(Input::Event::Type::Release, keycode));
		TrimBuffer(m_KeyBuffer);
	}

	void Input::OnChar(char character)
	{
		m_CharBuffer.push(character);
		TrimBuffer(m_CharBuffer);
	}

#pragma endregion

	Input::Event ReadBuffer(std::queue<Input::Event>& buffer)
	{
		if (buffer.size() > 0u)
		{
			Input::Event e = buffer.front();
			buffer.pop();
			return e;
		}
		else
		{
			return Input::Event();
		}
	}

	void UpdateBufferAndStates(std::queue<unsigned char>& updateQueue, Input::KeyState* state, std::queue<Input::Event>& buffer)
	{
		// update pressed and released to down and up
		for (uint32_t i = 0; i < updateQueue.size(); i++)
		{
			unsigned char key = updateQueue.front();
			updateQueue.pop();

			if (state[key] == Input::KeyState::Pressed)
				state[key] = Input::KeyState::Down;
			else if (state[key] == Input::KeyState::Released)
				state[key] = Input::KeyState::Up;
		}

		// update pressed and relesed events
		for (uint32_t i = 0; i < buffer.size(); i++)
		{
			Input::Event e = ReadBuffer(buffer);
			if (e.GetType() == Input::Event::Type::Press)
			{
				state[e.GetCode()] = Input::KeyState::Pressed;
			}
			else if (e.GetType() == Input::Event::Type::Release)
			{
				state[e.GetCode()] = Input::KeyState::Released;
			}

			updateQueue.push(e.GetCode());
		}
	}

	void Input::UpdateKeyStates()
	{
		UpdateBufferAndStates(m_UpdatedKeys, m_KeyState, m_KeyBuffer);
		UpdateBufferAndStates(m_UpdatedButtons, m_ButtonState, m_MouseBuffer);
	}


	void Input::ClearKeyStates()
	{
		for (uint32_t i = 0; i < sizeof(m_KeyState); i++)
		{
			m_KeyState[i] = KeyState::Up;
		}
	}

}
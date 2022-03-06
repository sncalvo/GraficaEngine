#include "Input.h"

#include <iostream>

namespace Engine
{
	Input::Input():
		_keyboard(nullptr),
		_mouse(0),
		_mouseX(0),
		_mouseY(0),
		_will_quit(false),
		_isLocked(false),
		_keyDown(),
		_keyUp(),
		_mouseDown(),
		_mouseUp()
	{}

	void Input::lock()
	{
		_isLocked = true;
	}

	void Input::unlock()
	{
		_isLocked = false;
	}

	void Input::update()
	{
		int i;
		for (i = 0; i < KEYBOARD_SIZE; i++)
		{
			_keyDown[i] = false;
			_keyUp[i] = false;
		}
		for (i = 0; i < MOUSE_MAX; i++)
		{
			_mouseDown[i] = false;
			_mouseUp[i] = false;
		}
		_movementX = 0;
		_movementY = 0;

		// Get key events from the OS
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				_will_quit = true;
				break;
			case SDL_KEYDOWN:
			{
				_keyboard = SDL_GetKeyboardState(nullptr);

				int index = event.key.keysym.scancode;

				_keyDown[index] = true;
			}
				break;
			case SDL_KEYUP:
			{
				_keyboard = SDL_GetKeyboardState(nullptr);

				int index = event.key.keysym.scancode;
				_keyUp[index] = true;
			}
				break;
			case SDL_MOUSEMOTION:
				_movementX = event.motion.xrel;
				_movementY = -event.motion.yrel;
				_mouseX = event.motion.x;
				_mouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				_mouse = SDL_GetMouseState(&(_mouseX), &(_mouseY));

				if (event.button.button == SDL_BUTTON_LEFT)
					_mouseDown[MOUSE_LEFT] = true;

				else if (event.button.button == SDL_BUTTON_RIGHT)
					_mouseDown[MOUSE_RIGHT] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				_mouse = SDL_GetMouseState(&(_mouseX),
					&(_mouseY));

				if (event.button.button == SDL_BUTTON_LEFT)
					_mouseUp[MOUSE_LEFT] = true;

				else if (event.button.button == SDL_BUTTON_RIGHT)
					_mouseUp[MOUSE_RIGHT] = true;
				break;
			default:
				break;
			}
		}
	}

	bool Input::getKeyDown(KeyboardKey key)
	{
		if (_isLocked) return false;

		if (key < 0 || key >= KEYBOARD_SIZE)
			return false;

		return _keyDown[key];
	}

	bool Input::getKeyUp(KeyboardKey key)
	{
		if (_isLocked) return false;

		if (key < 0 || key >= KEYBOARD_SIZE)
			return false;

		return _keyUp[key];
	}

	bool Input::getKey(KeyboardKey key)
	{
		if (_isLocked) return false;

		if (!_keyboard)
			return false;

		int sdl_key = static_cast<int>(key);

		if (_keyboard[sdl_key])
			return true;

		return false;
	}

	bool Input::getMouseButton(MouseButton button)
	{
		if (_isLocked) return false;

		switch (button)
		{
		case MOUSE_LEFT:
			if (_mouse & SDL_BUTTON(1))
				return true;
			break;

		case MOUSE_RIGHT:
			if (_mouse & SDL_BUTTON(3))
				return true;
			break;

		default:
			break;
		}

		return false;
	}

	std::tuple<int, int> Input::getMouseMovement()
	{
		return std::tuple<int, int>{_movementX, _movementY};
	}

	int Input::getMouseX()
	{
		return _mouseX;
	}

	int Input::getMouseY()
	{
		return _mouseY;
	}
}

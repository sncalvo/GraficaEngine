#pragma once

#include <tuple>

#include <SDL/SDL.h>

#include "InputDefinition.h"

namespace Engine
{
    class Input
    {
    private:
	    static SDL_Event _event;
        const uint8_t* _keyboard;
        uint32_t _mouse;
        int _mouseX;
        int _mouseY;
        int _movementX;
        int _movementY;
        bool _keyDown[KEYBOARD_SIZE];
        bool _keyUp[KEYBOARD_SIZE];
        bool _mouseDown[MOUSE_MAX];
        bool _mouseUp[MOUSE_MAX];
        bool _will_quit;
        bool _isLocked;

        Input();
    public:
        static Input& getInstance()
        {
            static Input _instance;
            return _instance;
        }

        void lock();
        void unlock();
	    void update();
        bool getKey(KeyboardKey key);
	    bool getKeyDown(KeyboardKey key);
        bool getKeyUp(KeyboardKey key);
        bool getMouseButton(MouseButton button);
        int getMouseX();
        int getMouseY();
        std::tuple<int, int> getMouseMovement();

        // Deleted functions are disabled by the compiler
        Input(Input const&) = delete;
        void operator=(Input const&) = delete;
    };
}

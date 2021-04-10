#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <GL/GLU.h>

constexpr GLdouble Z_NEAR = 0.1;
constexpr GLdouble Z_FAR = 100;

class Camera
{
public:
	Camera(int width, int heigth);
};

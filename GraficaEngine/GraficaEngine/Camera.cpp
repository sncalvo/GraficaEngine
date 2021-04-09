#include "Camera.h"

Camera::Camera(int width, int heigth)
{
	glMatrixMode(GL_PROJECTION);

	float color = 0.f;
	glClearColor(color, color, color, 1);

	gluPerspective(45, GLdouble(width) / GLdouble(heigth), Z_NEAR, Z_FAR);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}

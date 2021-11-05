#include "Camera.h"

CAMERA::CAMERA()
{
	x = 0;
	y = 3232;
}

void CAMERA::setx(int _x)
{
	x = _x;
}

void CAMERA::sety(int _y)
{
	y = _y;
}

int CAMERA::getx()
{
	return x;
}

int CAMERA::gety()
{
	return y;
}

void CAMERA::CameraSetting(WPARAM wParam)
{
	if (wParam == VK_LEFT)
	{

		return;
	}
	if (wParam == VK_RIGHT)
	{
		

		return;
	}
	if (wParam == VK_UP)
	{
		if(y>0)
		y -= 20;
		return;
	}
	if (wParam == VK_DOWN)
	{
		if(y+768<4096)
		y += 20;
		return;
	}
}


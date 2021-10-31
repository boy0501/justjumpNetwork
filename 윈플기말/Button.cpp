#include "Button.h"
#include <iostream>
Button::Button(std::function<void()> click, const int& x, const int& y,const int& width,const int& height,const COLORREF& transparantcolor)
	: mClick(click)
	, mPosx(x)
	, mPosy(y)
	, mWidth(width)
	, mHeight(height)
	, mCRTransparent(transparantcolor)
	, mOnButton(1)
{

}

Button::~Button()
{
	if (hbit[0]) DeleteObject(hbit[0]);
	if (hbit[1]) DeleteObject(hbit[1]);
	if (hbit[2]) DeleteObject(hbit[2]);
	if (hbit[3]) DeleteObject(hbit[3]);
}

void Button::initailize()
{

}

void Button::draw(HDC& mem1dc)
{
	HDC tmpdc;
	tmpdc = CreateCompatibleDC(mem1dc);
	auto oldtmp = SelectObject(tmpdc, hbit[mOnButton]);
	TransparentBlt(mem1dc, mPosx, mPosy, mWidth, mHeight, tmpdc, 0, 0, mWidth, mHeight, mCRTransparent);

	DeleteObject(tmpdc);

}

void Button::LoadButtonBitmap(const HINSTANCE& g_hinst, std::string name, std::string name1, std::string name2, std::string name3)
{
	auto tmp = std::wstring(name.begin(), name.end());
	hbit[0] = ((HBITMAP)LoadImage(g_hinst, tmp.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
	tmp = std::wstring(name1.begin(), name1.end());
	hbit[1] = ((HBITMAP)LoadImage(g_hinst, tmp.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
	tmp = std::wstring(name2.begin(), name2.end());
	hbit[2] = ((HBITMAP)LoadImage(g_hinst, tmp.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
	tmp = std::wstring(name3.begin(), name3.end());
	hbit[3] = ((HBITMAP)LoadImage(g_hinst, tmp.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
}

bool Button::containMouse(const int& mx, const int& my) const
{
	return !(
		mx < mPosx  ||
		mx >(mPosx + mWidth) ||
		my < mPosy ||
		my >(mPosy + mHeight)
		);
}

void Button::click()
{
	if (mClick)
	{
		mClick();
	}
}


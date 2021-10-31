#pragma once
#include <string>
#include <memory>
#include <functional>
#include <windows.h>

class Button
{
public:
	Button(std::function<void()> click, const int& x, const int& y,const int& width,const int& height,const COLORREF& transparentcolor);
	~Button();

	virtual void initailize();

	void draw(HDC& mem1dc);

	bool containMouse(const int& mx, const int& my) const;
	void LoadButtonBitmap(const HINSTANCE& g_hinst, std::string name, std::string name1, std::string name2, std::string name3);
	void click();

private:
	HBITMAP hbit[4];
	int mPosx;
	int mPosy;
	int mWidth;
	int mHeight;
	COLORREF mCRTransparent;
	std::function<void()> mClick;
	char mOnButton;	// 0 - 비활성화 , 1 - 기본 , 2 - hovered, 3 - clicked 
	bool mIsUserUi;
public:
	//true면 enabled false면 disabled
	bool IsOnButtonEnabled(const char OnButton) { return hbit[OnButton] != NULL; }
	// 0 - 비활성화 , 1 - 기본 , 2 - hovered, 3 - clicked 
	char getOnButton() const { return mOnButton; }
	// 0 - 비활성화 , 1 - 기본 , 2 - hovered, 3 - clicked 
	void setOnButton(char on) { mOnButton = on; } 
};

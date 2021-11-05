#include "Text.h"
#include "Camera.h"
Text::Text(const std::string& text, const std::string& NameTag, const std::wstring& fontName,const COLORREF& fontColor,const int& FontHeight, const int& x, const int& y,const bool& isUserUi,const int& StringBoxWidth,const int& StringBoxHeight, CAMERA& camera)
	:mText(text.begin(),text.end())
	,mNameTag(NameTag)
	,mFontName(fontName)
	,mFontColor(fontColor)
	, mFontHeight(FontHeight)
	,mPosx(x)
	,mPosy(y)
	,mIsUserUi(isUserUi)
	,mCamera(&camera)
	,mStringBoxWidth(StringBoxWidth)
	,mStringBoxHeight(StringBoxHeight)
{

}

Text::Text(const std::wstring& text, const std::string& NameTag, const std::wstring& fontName, const COLORREF& fontColor, const int& FontHeight, const int& x, const int& y, const bool& isUserUi, const int& StringBoxWidth, const int& StringBoxHeight, CAMERA& camera)
	:mText(text)
	, mNameTag(NameTag)
	, mFontName(fontName)
	, mFontColor(fontColor)
	, mFontHeight(FontHeight)
	, mPosx(x)
	, mPosy(y)
	, mIsUserUi(isUserUi)
	, mCamera(&camera)
	, mStringBoxWidth(StringBoxWidth)
	, mStringBoxHeight(StringBoxHeight)
{

}

Text::~Text()
{

}

void Text::draw(HDC& mem1dc)
{
	
	HFONT hfont = CreateFont(mFontHeight, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, mFontName.c_str());
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);

	auto oldFontColor = SetTextColor(mem1dc, mFontColor);
	//std::cout << id << std::endl;
	if (mIsUserUi)
	{
		SetBkMode(mem1dc, TRANSPARENT);		
		RECT rt{ mCamera->getx() + mPosx,mCamera->gety() + mPosy,mCamera->getx() + mPosx + mStringBoxWidth,mCamera->gety() + mPosy + mStringBoxHeight };
		DrawText(mem1dc, mText.c_str(), lstrlenW(mText.c_str()), &rt, DT_LEFT);
	}
	else {
		TextOut(mem1dc, mPosx, mPosy, mText.c_str(), lstrlenW(mText.c_str()));
	}
	SetTextColor(mem1dc, oldFontColor);
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
}

void Text::UpdateFontSize(HWND& hwnd)
{
	HDC mem1dc = GetDC(hwnd);	//임시용 HDC Create 
	HFONT hfont = CreateFont(mFontHeight, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, mFontName.c_str());	//폰트크기체크용 폰트 Create
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);

	GetTextExtentPoint(mem1dc, mText.c_str(), lstrlenW(mText.c_str()), &mFontLength);

	SelectObject(mem1dc, oldfont);	//기존 brush로 복구
	DeleteObject(hfont);			//Create 했던것 Delete
	ReleaseDC(hwnd, mem1dc);		//Create 했던것 Delete
}


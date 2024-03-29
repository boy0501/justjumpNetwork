#pragma once
#include <string>
#include <windows.h>
class Text
{
public:
	Text(const std::string& text,const std::string& NameTag, const std::wstring& fontName,const COLORREF& fontColor,const int& FontHeight,const int& x, const int& y,const bool& IsUserUi,const int& StringBoxWidth,const int& StringBoxHeight, class CAMERA& camera);
	Text(const std::wstring& text, const std::string& NameTag, const std::wstring& fontName, const COLORREF& fontColor, const int& FontHeight, const int& x, const int& y, const bool& isUserUi, const int& StringBoxWidth, const int& StringBoxHeight, CAMERA& camera);

	~Text();


	void draw(HDC& mem1dc);

	
	void UpdateFontSize(HWND& hwnd);
	void pushByteChar(const char& c) { mText.pop_back(); mText += c; }
	void pushChar(const char& c) { mText += c; }
	void pushString(const std::wstring s) { mText += s + L"   ";}
	void pushwChar(const wchar_t& c) { mText += c; }
	void changewChar(const wchar_t& c) { if (mText.size() == 0) mText[0] = c; else mText[mText.size() - 1] = c; }	//한글입력할때를 위한 함수.
	void popChar() { mText.pop_back(); }
	void setText(const std::wstring s) { mText = s; }

	const int& getTextLen() const { return mText.length(); }
	const SIZE& getFontLen() const { return mFontLength; }
	const std::string getNameTag() const { return mNameTag; }
	const std::wstring getText() const { return mText; }
	const std::string getTextForString() const;
private:
	int mPosx;
	int mPosy;
	int mFontHeight;
	int mStringBoxWidth;
	int mStringBoxHeight;
	bool mIsUserUi;
	class CAMERA* mCamera;
	std::wstring mFontName;
	std::wstring mText;
	std::string mNameTag;
	COLORREF mFontColor;
	SIZE mFontLength;

public:
};


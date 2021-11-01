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
	void pushwChar(const wchar_t& c) { mText += c; }
	void changewChar(const wchar_t& c) { mText[mText.size()-1] = c; }
	void popChar() { mText.pop_back(); }
	const int& getTextLen() const { return mText.length(); }
	const SIZE& getFontLen() const { return mFontLength; }
	const std::string getNameTag() const { return mNameTag; }
	const std::wstring getText() const { return mText; }
	const std::string getTextForString() const { return std::string(mText.begin(),mText.end()); }
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


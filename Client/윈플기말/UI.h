#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <windows.h>

struct UIINFO {
	int mx, my, mw, mh;
	COLORREF mcr;
	bool mIsUserUi;	//true면 유저시점으로 그려지는 ui false면 화면시점
	UIINFO(const int& x, const int& y, const int& w, const int&  h, const COLORREF& cr,const bool& isUserui)
		: mx(x)
		, my(y)
		, mw(w)
		, mh(h)
		, mcr(cr)
		, mIsUserUi(isUserui)
	{

	}
};

class UI : public std::enable_shared_from_this<UI>
{
public:
	enum class UIState
	{
		Active,
		Dead
	};

	UI(const int& cnt);
	virtual ~UI() noexcept;


	virtual void processInput(const int& mx, const int& my,const int& command);
	virtual void update(float deltatime);
	virtual void draw(HDC& mem1dc);
	virtual void drawExit(HDC& mem1dc);
	virtual void drawRanking(HDC& mem1dc);

	void drawByUserUi(HDC& mem1dc);
	void drawByScreenUi(HDC& mem1dc);
	void LoadUiBitmap(HINSTANCE& g_hinst, std::string name, const int& x, const int& y, const int& w, const int& h, const COLORREF& cr,class CAMERA& camera);
	void LoadUiBitmap(HINSTANCE& g_hinst, std::string name, const int& x, const int& y, const int& w, const int& h, const COLORREF& cr);
	void addButton(std::function<void()> click, const HINSTANCE& g_hinst, const std::string& fileName, const int& x, const int& y, const int& width, const int& height, const COLORREF& transparentcolor);
	void addText(const std::string& text, const std::string& NameTag, const std::wstring& fontName, const COLORREF& fontColor, const int& FontHeight, const int& x, const int& y, const bool& isUserUi, const int& StringBoxWidth, const int& StringBoxHeight,class CAMERA& camera);
	void addText(const std::wstring& text, const std::string& NameTag, const std::wstring& fontName,const COLORREF& fontColor, const int& FontHeight, const int& x, const int& y, const bool& isUserUi,	const int& StringBoxWidth, const int& StringBoxHeight, CAMERA& camera);
	void closeUI();

protected:
	class CAMERA* mCamera;
	UIState mState;
	std::vector<std::shared_ptr<class Button>> mButtons;
	std::vector<std::shared_ptr<class Text>> mText;
	int mUiCount;
	std::vector<HBITMAP> hbits;
	std::vector<UIINFO> mUiInfo;


public:
	std::shared_ptr<class Text> FindTextByNameTag(std::string NameTag);
	UIState getState() const { return mState; }
};
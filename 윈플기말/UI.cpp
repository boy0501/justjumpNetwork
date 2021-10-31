#include "UI.h"
#include "Button.h"
#include "Text.h"
#include "Sound.h"
#include "Camera.h"
UI::UI(const int& cnt)
	: mState(UIState::Active)
	, mUiCount(cnt)
	, mCamera(NULL)
{

}

UI::~UI()
{
	for (auto& button : mButtons)
	{
		button.reset();
	}
	mButtons.clear();
	for (auto& hbit : hbits)
	{
		if (hbit) DeleteObject(hbit);
	}
	hbits.clear();
	mUiInfo.clear();
}

void UI::LoadUiBitmap(HINSTANCE& g_hinst,std::string name,const int& x,const int& y, const int& w, const int& h, const COLORREF& cr)
{
	if (hbits.size() > mUiCount)
	{
		std::cout << "허용 ui카운트 보다 많은 ui를 삽입하려고 시도함" << std::endl;
		return;
	}
	auto tmp = std::wstring(name.begin(), name.end());
	auto tmp_convert_wstr = tmp.c_str();
	hbits.push_back((HBITMAP)LoadImage(g_hinst, tmp_convert_wstr, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
	mUiInfo.push_back(UIINFO(x, y, w, h,cr,false));
}

void UI::LoadUiBitmap(HINSTANCE& g_hinst, std::string name, const int& x, const int& y, const int& w, const int& h, const COLORREF& cr,CAMERA& camera)
{
	if (hbits.size() > mUiCount)
	{
		std::cout << "허용 ui카운트 보다 많은 ui를 삽입하려고 시도함" << std::endl;
		return;
	}
	auto tmp = std::wstring(name.begin(), name.end());
	auto tmp_convert_wstr = tmp.c_str();
	hbits.push_back((HBITMAP)LoadImage(g_hinst, tmp_convert_wstr, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
	mUiInfo.push_back(UIINFO(x, y, w, h, cr,true));
	mCamera = &camera;
}

void UI::processInput(const int& mx, const int& my,const int& command)
{

	for (const auto& button : mButtons)
	{
		if (button->containMouse(mx,my))
		{
			if (command == WM_LBUTTONDOWN)
			{
				if (!button->IsOnButtonEnabled(3))
					continue;
				if (button->getOnButton() != 3)
					Sound::GetSelf()->Sound_Play(EFFECTSOUND, MCLICKEF, EFVOL);
				button->setOnButton(3);
			}
			else
			{
				if (!button->IsOnButtonEnabled(2))
					continue;
				if(button->getOnButton()!=2)
					Sound::GetSelf()->Sound_Play(EFFECTSOUND, MOVEREF, EFVOL);
				button->setOnButton(2);
			}
		}
		else
		{
			if (!button->IsOnButtonEnabled(1))
				continue;
			button->setOnButton(1);
		}
	}

	auto click = WM_LBUTTONUP == command;
	if (click)
	{
		for (const auto& button : mButtons)
		{
			if (button->getOnButton() >= 2)
			{
				button->click();
			}
		}
	}
}
void UI::update(float deltatime)
{

}
void UI::draw(HDC& mem1dc)
{
	for (const auto& txt : mText)
	{
		txt->draw(mem1dc);
	}
	for (const auto& button : mButtons)
	{
		button->drawByScreenButton(mem1dc);
	}
}
void UI::drawByUserUi(HDC& mem1dc)
{
	HDC tmpdc;
	tmpdc = CreateCompatibleDC(mem1dc);
	for (int i = 0; i < mUiCount; ++i)
	{
		auto info = mUiInfo[i];
		SelectObject(tmpdc, hbits[i]); 
		TransparentBlt(mem1dc, mCamera->getx() + info.mx, mCamera->gety() + info.my, info.mw, info.mh, tmpdc, 0, 0, info.mw, info.mh, info.mcr);

	}
	DeleteObject(tmpdc);
}
void UI::drawByScreenUi(HDC& mem1dc)
{
	HDC tmpdc;
	tmpdc = CreateCompatibleDC(mem1dc);
	for (int i = 0; i < mUiCount; ++i)
	{
		auto info = mUiInfo[i];
		SelectObject(tmpdc, hbits[i]); TransparentBlt(mem1dc, info.mx, info.my, info.mw, info.mh, tmpdc, 0, 0, info.mw, info.mh, info.mcr);

		
	}
	DeleteObject(tmpdc);
}
void UI::addButton(std::function<void()> click,const HINSTANCE& g_hinst,const std::string& fileName, const int& x, const int& y, const int& width, const int& height, const COLORREF& transparentcolor)
{
	auto button = std::make_shared<Button>(click, x, y, width, height, transparentcolor);
	button->LoadButtonBitmap(g_hinst, fileName + "0.bmp",fileName + "1.bmp",fileName + "2.bmp",fileName + "3.bmp");
	mButtons.emplace_back(button);
}
void UI::addText(const std::string& text, const std::string& NameTag, const std::wstring& fontName, 
	const COLORREF& fontColor, const int& FontHeight, const int& x, const int& y, const bool& isUserUi, 
	const int& StringBoxWidth, const int& StringBoxHeight, CAMERA& camera)
{
	auto txt = std::make_shared<Text>(text, NameTag, fontName, fontColor, FontHeight, x, y,isUserUi,StringBoxWidth,StringBoxHeight,camera);
	mText.emplace_back(txt);

}
std::shared_ptr<Text> UI::FindTextByNameTag(std::string NameTag)
{
	for (const auto& txt : mText)
	{
		if (txt->getNameTag() == NameTag)
			return txt;
	}

	return nullptr;
}
void UI::closeUI()
{
	mState = UIState::Dead;
}

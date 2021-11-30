#include "DieHUD.h"
#include "player.h"
#include "Button.h"
#include "Camera.h"
DieHUD::DieHUD(const int& cnt, PLAYER& player,CAMERA& camera)
	:UI(cnt)
	, mPlayer(&player)
{
	mCamera = &camera;
}

DieHUD::~DieHUD()
{
	if (mHpBitmap) DeleteObject(mHpBitmap);
	std::cout << "dd" << std::endl;
}



void DieHUD::draw(HDC& mem1dc)
{

	drawByScreenUi(mem1dc);
	drawDieButton(mem1dc);
	drawDieText(mem1dc);	
	for (const auto& button : mButtons)
	{
		button->drawByScreenButton(mem1dc);
	}
}

void DieHUD::drawDieText(HDC& mem1dc)
{
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 bold"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);

	SetTextColor(mem1dc, RGB(32, 108, 168));
	TextOut(mem1dc, mCamera->getx() + 459, mCamera->gety() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, mCamera->getx() + 461, mCamera->gety() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, mCamera->getx() + 460, mCamera->gety() + 259, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, mCamera->getx() + 460, mCamera->gety() + 261, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));

	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, mCamera->getx() + 460, mCamera->gety() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);

	hfont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
	oldfont = (HFONT)SelectObject(mem1dc, hfont);
	TextOut(mem1dc, mCamera->getx() + 460, mCamera->gety() + 285, L"아쉽지만 확인을 누르시면 맵의 맨 처음", lstrlenW(L"아쉽지만 확인을 누르시면 맵의 맨 처음."));
	TextOut(mem1dc, mCamera->getx() + 460, mCamera->gety() + 297, L"위치로 돌아가게 됩니다. 포기하지 마세요!", lstrlenW(L"위치로 돌아가게 됩니다. 포기하지 마세요!"));
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
}
void DieHUD::drawDieButton(HDC& mem1dc)
{
	for (const auto& button : mButtons)
	{
		button->drawByUserButton(mem1dc, mCamera);
	}
/*
	HDC tmpdc;
	tmpdc = CreateCompatibleDC(mem1dc);
	auto oldtmp = SelectObject(tmpdc, hbit[mOnButton]);
	TransparentBlt(mem1dc, mPosx, mPosy, mWidth, mHeight, tmpdc, 0, 0, mWidth, mHeight, mCRTransparent);

	DeleteObject(tmpdc);*/
}

void DieHUD::update(float deltatime)
{

}


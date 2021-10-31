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
	drawDieButton(mem1dc);
	drawDieText(mem1dc);	
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
	}/*
	HDC tmpdc;
	tmpdc = CreateCompatibleDC(mem1dc);
	auto oldtmp = SelectObject(tmpdc, hbit[mOnButton]);
	TransparentBlt(mem1dc, mPosx, mPosy, mWidth, mHeight, tmpdc, 0, 0, mWidth, mHeight, mCRTransparent);

	DeleteObject(tmpdc);*/
}

void DieHUD::update(float deltatime)
{

}


void DieHUD::drawHP(HDC& mem1dc)
{
	int hp = mPlayer->gethp() * 171 / 100;
	TCHAR hpname[100];
	_itow_s(mPlayer->gethp(), hpname, 10);
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
	HDC mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, mHpBitmap);
	BitBlt(mem1dc, mCamera->getx() + 421, mCamera->gety() + 728, hp, 65, mem2dc, 0, 0, SRCCOPY);
	SetBkMode(mem1dc, 1);
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, mCamera->getx() + 481, mCamera->gety() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, mCamera->getx() + 483, mCamera->gety() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, mCamera->getx() + 482, mCamera->gety() + 727, hpname, lstrlenW(hpname));
	TextOut(mem1dc, mCamera->getx() + 482, mCamera->gety() + 729, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, mCamera->getx() + 482, mCamera->gety() + 728, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, mCamera->getx() + 504, mCamera->gety() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, mCamera->getx() + 506, mCamera->gety() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, mCamera->getx() + 505, mCamera->gety() + 727, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, mCamera->getx() + 505, mCamera->gety() + 729, L"/100", lstrlenW(L"/100"));
	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, mCamera->getx() + 505, mCamera->gety() + 728, L"/100", lstrlenW(L"/100"));
	//StretchBlt(mem1dc, camera.getx() + 421, camera.gety() + 728, hp, 65, mem2dc, 0, 0,hp, 65,SRCCOPY);
	//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//HP 전체 새로고침
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
	DeleteObject(mem2dc);
}

void DieHUD::LoadHpUiBitmap(HINSTANCE& g_hinst, std::string name, const int& x, const int& y, const int& w, const int& h, const COLORREF& cr, CAMERA& camera)
{

	auto tmp = std::wstring(name.begin(), name.end());
	auto tmp_convert_wstr = tmp.c_str();
	mHpBitmap = (HBITMAP)LoadImage(g_hinst, tmp_convert_wstr, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	mUiInfo.push_back(UIINFO(x, y, w, h, cr, true));
}

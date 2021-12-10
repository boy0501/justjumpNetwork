#include "LoginHUD.h"
#include "Button.h"
#include "Text.h"
#include "Map.h"
#include "Network.h"

LoginHUD::LoginHUD(const int& cnt,MAP* map)
	:UI(cnt)
	,mMap(map)
{

}

LoginHUD::~LoginHUD()
{

}



void LoginHUD::draw(HDC& mem1dc)
{
	drawByScreenUi(mem1dc);
	for (const auto& button : mButtons)
	{
		button->drawByScreenButton(mem1dc);
	}

	UI::draw(mem1dc);
}


void LoginHUD::update(float deltatime)
{

}


void LoginHUD::processInput(const int& mx, const int& my, const int& command)
{
	if (command == WM_LBUTTONDOWN)
	{
		if (mx > 365 && mx < 565)
		{
			if (my > 320 && my < 360)
			{
				mMap->LoginInputFlag = false;
				Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->UpdateFontSize(hWnd);
				auto CaretX = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("id")->getFontLen().cx;
				auto CaretY = 330;
				SetCaretPos(CaretX, CaretY);
			}
			if (my > 360 && my < 400)
			{

				mMap->LoginInputFlag = true;
				Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->UpdateFontSize(hWnd);
				auto CaretX = 380 + Network::GetNetwork()->mUI.back()->FindTextByNameTag("ip")->getFontLen().cx;
				auto CaretY = 380;
				SetCaretPos(CaretX, CaretY);
			}
		}
	}
	UI::processInput(mx, my, command);
}
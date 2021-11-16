#include "LoginHUD.h"
#include "Button.h"

LoginHUD::LoginHUD(const int& cnt)
	:UI(cnt)
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
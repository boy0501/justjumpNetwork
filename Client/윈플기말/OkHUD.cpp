#include "OkHUD.h"
#include "Button.h"

OkHUD::OkHUD(const int& cnt)
	:UI(cnt)
{

}

OkHUD::~OkHUD()
{

}



void OkHUD::draw(HDC& mem1dc)
{
	drawByScreenUi(mem1dc);
	for (const auto& button : mButtons)
	{
		button->drawByScreenButton(mem1dc);
	}
	UI::draw(mem1dc);
}


void OkHUD::update(float deltatime)
{

}
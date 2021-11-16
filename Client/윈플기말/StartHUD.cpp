#include "StartHUD.h"
#include "Button.h"

StartHUD::StartHUD(const int& cnt)
	:UI(cnt)
{

}

StartHUD::~StartHUD()
{

}



void StartHUD::draw(HDC& mem1dc)
{
	drawByScreenUi(mem1dc);
	for (const auto& button : mButtons)
	{
		button->drawByScreenButton(mem1dc);
	}
	UI::draw(mem1dc);
}


void StartHUD::update(float deltatime)
{

}
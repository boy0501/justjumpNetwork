#include "ExitGameHUD.h"

ExitGameHUD::ExitGameHUD(const int& cnt)
	:UI(cnt)
{
}

ExitGameHUD::~ExitGameHUD()
{
}

void ExitGameHUD::draw(HDC& mem1dc)
{
	drawByScreenUi(mem1dc);
	UI::draw(mem1dc);
}

void ExitGameHUD::update(float deltatime)
{

}
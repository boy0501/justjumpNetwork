#include "ExitHUD.h"

ExitHUD::ExitHUD(const int& cnt)
	:UI(cnt)
{

}

ExitHUD::~ExitHUD()
{

}

int ExitHUD::getMapNum()
{
	return mapNum;
}


void ExitHUD::draw(HDC& mem1dc)
{
	drawByScreenUi(mem1dc);
	UI::draw(mem1dc);

	//if (try_once == false) {
	this->addButton([this]() {
		}
	, NULL, "img/Exit", 315, 3400, 138, 82, RGB(255, 0, 0));
	//try_once = true;
	//}
	std::cout << "yes" << std::endl;
}


void ExitHUD::update(float deltatime)
{

}
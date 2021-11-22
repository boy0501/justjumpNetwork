
#include "Object.h"


Object::Object()
	: x(0),y(0),w(0),h(0)
	,type(0)
	,dir(0)
	,index(0)
{
}

Object::~Object()
{

}


void Object::initalize(int _x, int _y, int _w, int _h, int _type)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	type = _type;
}
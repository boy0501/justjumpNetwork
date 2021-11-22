#pragma once
class Object
{
public:
	Object();
	virtual ~Object();
	virtual void update(float deltatime);

	//쓰려고 연구중... 아직 안 쓰임
	enum class Type
	{
		NONE,
		LAND,
		NORMALPLATFORM,
		SMALLPLAT,
		CONVAYIOR,
		GLASS,
		DRILL = 101,
		PIPE = 102,
		STEAM = 103,
		ROWGEAR = 106,
		COLGEAR = 107,
		PORTAL = 201,
		ROPE = 301
	};

	int x, y;					//오브젝트의 x y 는 왼쪽상단이 기준이다.
	int w, h;
	int type;					//1.바닥 2.기본 발판 3.작은발판 4.컨베이어 벨트 5.투명바닥 101.나사못 102.깨진파이프 103.증기 106,107.톱니바퀴 201.포탈 301.로프
	int mx, my;					//Variables that increase/decrease based on the object's x,y coordinates
	int s;						//sign bit

	void initalize(int _x, int _y, int _w, int _h, int _type);

};


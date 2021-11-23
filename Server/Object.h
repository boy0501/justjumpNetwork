#pragma once
class Object
{
public:
	Object();
	virtual ~Object();
	virtual void update(float deltatime);

	//������ ������... ���� �� ����
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

	int x, y;					//������Ʈ�� x y �� ���ʻ���� �����̴�.
	int w, h;
	int type;					//1.�ٴ� 2.�⺻ ���� 3.�������� 4.�����̾� ��Ʈ 5.����ٴ� 101.����� 102.���������� 103.���� 106,107.��Ϲ��� 201.��Ż 301.����
	int mx, my;					//Variables that increase/decrease based on the object's x,y coordinates
	int s;						//sign bit

	void initalize(int _x, int _y, int _w, int _h, int _type);

};


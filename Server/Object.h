#pragma once
class Object
{
public:
	Object();
	~Object();

	int x, y;					//������Ʈ�� x y �� ���ʻ���� �����̴�.
	int w, h;
	int type;					//1.�ٴ� 2.�⺻ ���� 3.�������� 4.�����̾� ��Ʈ 5.����ٴ� 101.����� 102.���������� 103.���� 106,107.��Ϲ��� 201.��Ż 301.����
	int index;					//�ִϸ��̼� ������ �迭�� �ε����� �ٲ��־� �̹����� �ٲ۴�
	int mx, my;					//Variables that increase/decrease based on the object's x,y coordinates
	int s;						//sign bit
	int dir;					//object's moving direction 0 == left , right / 1 == up , down

	void initalize(int _x, int _y, int _w, int _h, int _type);

};


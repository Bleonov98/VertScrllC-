#pragma once
#include "Tools.h"

class GameObject
{
public:
	GameObject(wd* wData, int x, int y, int speed, int color) {
		this->wData = wData;
		_x = x, _y = y, _speed = speed, _color = color;
	};

	int GetX();

	int GetY();

	int GetSpeed();

	int GetWidth();

	int GetHeight();

	void SetX(int x);

	void SetY(int y);



	virtual void DrawObject() = 0;

	virtual void EraseObject() = 0;

	virtual void MoveObject() = 0;



	void DeleteObject();

	bool IsObjectDelete();

protected:
	
	wd* wData;

	int _x, _y, _speed, _color, _direction = DOWN, _width, _height;

	bool _deleteObject = false;

};



class Character: public GameObject 
{
public:
	Character(wd* wData, int x, int y, int speed, int color):GameObject(wData, x, y , speed, color) {};

	void Hit();

private:

	int _gunSpeed = 2000;

};





class Player: public Character
{
public:
	Player(wd* Wdata, int x, int y, int speed, int color) : Character(wData, x, y, speed, color) {};

private:



};
#pragma once
#include "Tools.h"

static const int SMALL_WIDTH = 3;
static const int REGULAR_HEIGHT = 3;
static const int REGULAR_WIDTH = 5;
static const int BOSS_HEIGHT = 10;
static const int BOSS_WIDTH = 16;


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

	virtual ~GameObject() {
		delete this;
	}

};


class Character: public GameObject 
{
public:
	Character(wd* wData, int x, int y, int speed, int color):GameObject(wData, x, y , speed, color) {
		
	};


	int GetGunSpeed();

	void SetGunSpeed(int gunSpeed);


	void SetGunType(int gunType);

	int GetGunType();


	void SetGunState(bool reload);

	bool GetGunState();

	void ReloadGun();


	int GetHp();

protected:

	int _gunSpeed = 2000, _lifes = 1, _gunType = SHOT, _hp = 100;

	bool _reload = true;
};







class Player: public Character
{
public:
	Player(wd* wData, int x, int y, int speed, int color) : Character(wData, x, y, speed, color) {
		_width = REGULAR_WIDTH - 1;
		_height = REGULAR_HEIGHT;
		_gunSpeed = 500;
		_gunType = ROCKET;
	};


	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;


	void Death(bool& worldIsRun);


	int GetLifes();

	void AddLife();

private:

	char16_t playerSprite[REGULAR_HEIGHT][REGULAR_WIDTH]
	{
		u" !! ",
		u"|**|",
		u"'--'"
	};

};



class Enemy : public Character
{
public:
	Enemy(wd* Wdata, int x, int y, int speed, int color) : Character(wData, x, y, speed, color) {};


	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;


	void Hit();


	void SetEnemyType(int type);

	int GetEnemyType();


private:

	int _type = SMALL, _lifes = 1;

	char16_t smallEnemy[REGULAR_HEIGHT][SMALL_WIDTH]{
		u"--",
		u"--",
		u"##"
	};
	char16_t regularEnemy[REGULAR_HEIGHT][REGULAR_WIDTH]{
		u"|^|",
		u"|-|",
		u"# #"
	};
	char16_t landEnemy[REGULAR_HEIGHT][REGULAR_WIDTH]{
		u"   #",
		u")###",
		u"   #"
	};
	char16_t bossEnemy[BOSS_HEIGHT][BOSS_WIDTH]{
		u" ------------- ",
		u"| ====  ====  |",
		u"| ----  ----  |",
		u"|    ****     |",
		u"|   *    *    |",
		u"|    ****     |",
		u"| # # # # # # |",
		u"|-------------|",
		u"||   | | |   ||",
		u"||   | | |   ||"
	};

};



class Bullet : public GameObject
{
public:

	Bullet(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;


	int GetOwner();

	void SetOwner(int owner);
	

	void SetBulletType(int type);

	
	void RocketPath(int x, int y);

private:

	int _owner, _type;

	vector <pair<int, int>> path;

	bool targetMove = false;
};



class Bonus : public GameObject
{
public:

	Bonus(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {
		_width = SMALL_WIDTH - 1;
		_height = REGULAR_HEIGHT;
		_speed = 1;
	};

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;


	void SetBonusType(int type);

	int GetBonusType();

private:

	int _type = GUNSPEED;

	char16_t gunspeedSprite[REGULAR_HEIGHT][SMALL_WIDTH]{
		u"--",
		u"SP",
		u"--"
	};
	char16_t guntypeSprite[REGULAR_HEIGHT][SMALL_WIDTH]{
		u"--",
		u"TP",
		u"--"
	};
	char16_t lifeBonusSprite[REGULAR_HEIGHT][SMALL_WIDTH]{
		u"--",
		u"LF",
		u"--"
	};

};
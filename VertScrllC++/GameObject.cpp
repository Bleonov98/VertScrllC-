#include "GameObject.h"

int GameObject::GetX()
{
    return _x;
}

int GameObject::GetY()
{
    return _y;
}

int GameObject::GetSpeed()
{
    return _speed;
}

int GameObject::GetWidth()
{
    return _width;
}

int GameObject::GetHeight()
{
    return _height;
}

void GameObject::SetX(int x)
{
    _x = x;
}

void GameObject::SetY(int y)
{
    _y = y;
}

void GameObject::DeleteObject()
{
    EraseObject();
    _deleteObject = true;
}

bool GameObject::IsObjectDelete()
{
    return _deleteObject;
}


// ------------------ Char ------------------------


int Character::GetGunSpeed()
{
    return _gunSpeed;
}

void Character::SetGunSpeed(int gunSpeed)
{
    _gunSpeed = gunSpeed;
}

void Character::SetGunType(int gunType)
{
    _gunType = gunType;
}

int Character::GetGunType()
{
    return _gunType;
}

void Character::SetGunState(bool reload)
{
    _reload = reload;
}

bool Character::GetGunState()
{
    return _reload;
}

void Character::ReloadGun()
{
    _reload = false;

    Sleep(_gunSpeed);

    _reload = true;
}

int Character::GetHp()
{
    return _hp;
}

// -------------------- Player --------------------

void Player::DrawObject()
{
    for (int h = 0; h < _height; h++)
    {
        for (int w = 0; w < _width; w++)
        {
            wData->vBuf[_y + h][_x + w] = playerSprite[h][w] | (_color << 8);
        }
    }
    _color = BrBlue;
}

void Player::EraseObject()
{
    for (int h = 0; h < _height; h++)
    {
        for (int w = 0; w < _width; w++)
        {
            wData->vBuf[_y + h][_x + w] = u' ';
        }
    }
}

void Player::MoveObject()
{
    EraseObject();

    if (GetAsyncKeyState(VK_UP) && _y > 4) _y--;
    if (GetAsyncKeyState(VK_DOWN) && _y < ROWS - 4) _y++;
    if (GetAsyncKeyState(VK_RIGHT) && _x < COLS - 6) _x++;
    if (GetAsyncKeyState(VK_LEFT) && _x > 2) _x--;
}

void Player::Death(bool& worldIsRun)
{
    _hp -= 25;
    _color = BrPurple;
    if (_hp == 0) {
        _lifes--;
        if (_lifes == 0) {
            worldIsRun = false;
            PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);
            return;
        }
        _hp = 100;
        _gunType = SHOT;
        _gunSpeed = 500;
    }
}

int Player::GetLifes()
{
    return _lifes;
}

void Player::AddLifes()
{
    if (_hp == 100) {
        _lifes++;
    }
    else _hp += 25;
}

// ----------------- BULLET -----------------


void Bullet::DrawObject()
{
    if (_type == ROCKET) {
        wData->vBuf[_y][_x] = u'@' | (_color << 8);
    }
    else wData->vBuf[_y][_x] = u'|' | (_color << 8);
}

void Bullet::EraseObject()
{
    wData->vBuf[_y][_x] = u' ';
}

void Bullet::MoveObject()
{
    EraseObject();

    if (!targetMove) {
        if (_direction == UP) {
            _y--;
            if (_y <= 2) DeleteObject();
        }
        else if (_direction == DOWN) {
            _y++;
            if (_y >= ROWS - 1) DeleteObject();
        }
        else if (_direction == RIGHT) {
            _x++;
            if (_x >= COLS - 2) DeleteObject();
        }
        else if (_direction == LEFT) {
            _x--;
            if (_x <= 2) DeleteObject();
        }
    }
    else {
        if (!path.empty()) {
            SetX(path.back().first);
            SetY(path.back().second);

            path.pop_back();

            if (path.empty()) {
                DeleteObject();
            }
        }
    }
}

void Bullet::SetOwner(int owner)
{
    _owner = owner;

    if (_owner == PLAYER) {
        _direction = UP;
    }
    else if (_owner == ENEMY) {
        _direction = DOWN;
    }
    else if (_owner == LAND) {
        if (_x < COLS / 2) {
            _direction = RIGHT;
        }
        else _direction = LEFT;
    }
}

void Bullet::SetBulletType(int type)
{
    _type = type;
}

void Bullet::RocketPath(int x, int y)
{
    int X = _x, Y = _y;

    int dx = abs(x - X);
    int dy = abs(y - Y);

    int error = dx - dy;

    int dirX = (X < x) ? 1 : -1;
    int dirY = (Y < y) ? 1 : -1;

    while (X != x && Y != y)
    {
        const int error2 = error * 2;

        if (error2 > -dy)
        {
            error -= dy;
            X += dirX;
        }
        if (error2 < dx)
        {
            error += dx;
            Y += dirY;
        }

        path.insert(path.begin(), make_pair(X, Y));
    }

    targetMove = true;
}

int Bullet::GetOwner()
{
    return _owner;
}



// ------------------ ENEMY ------------------

void Enemy::DrawObject()
{
    if (_type == SMALL) {
        for (int h = 0; h < _height; h++)
        {
            for (int w = 0; w < _width; w++)
            {
                wData->vBuf[_y + h][_x + w] = smallEnemy[h][w] | (_color << 8);
            }
        }
    }
    else if (_type == REGULAR) {
        for (int h = 0; h < _height; h++)
        {
            for (int w = 0; w < _width; w++)
            {
                wData->vBuf[_y + h][_x + w] = regularEnemy[h][w] | (_color << 8);
            }
        }
    }
    else if (_type == LAND) {
        for (int h = 0; h < _height; h++)
        {
            for (int w = 0; w < _width; w++)
            {
                wData->vBuf[_y + h][_x + w] = landEnemy[h][w] | (_color << 8);
            }
        }
    }
    else if (_type == BOSS) {
        for (int h = 0; h < _height; h++)
        {
            for (int w = 0; w < _width; w++)
            {
                wData->vBuf[_y + h][_x + w] = bossEnemy[h][w] | (_color << 8);
            }
        }
    }
    _color = BrYellow;
}

void Enemy::EraseObject()
{
    for (int h = 0; h < _height; h++)
    {
        for (int w = 0; w < _width; w++)
        {
            wData->vBuf[_y + h][_x + w] = u' ';
        }
    }
}

void Enemy::MoveObject()
{
    EraseObject();

    if (_type == BOSS) {
        BossDir();
    }
    else {
        _y++;
        if (_y + _height >= ROWS - 2) {
            DeleteObject();
        }
    }
}

void Enemy::Hit(int& score)
{
    _hp -= 25;
    _color = BrPurple;
    if (_hp == 0) {
        _lifes--;
        if (_lifes == 0) {
            PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);
            DeleteObject();

            if (_type == SMALL) score += 50;
            if (_type == REGULAR) score += 100;
            if (_type == LAND) score += 250;
            if (_type == BOSS) score += 1000;
        }
    }
}

void Enemy::SetEnemyType(int type)
{
    _type = type;

    if (_type == SMALL) {
        _width = SMALL_WIDTH - 1;
        _height = REGULAR_HEIGHT;
        _gunSpeed = 1500;
        _gunType = SHOT;
        _hp = 25;
    }
    else if (_type == REGULAR) {
        _width = REGULAR_WIDTH - 1;
        _height = REGULAR_HEIGHT;
        _gunSpeed = 1000;
        _gunType = DOUBLESHOT;
        _hp = 50;
    }
    else if (_type == LAND) {
        _width = REGULAR_WIDTH - 1;
        _height = REGULAR_HEIGHT;
        _gunSpeed = 400;
        _gunType = ROCKET;
        _speed = 5;
        _hp = 25;
    }
    else if (_type == BOSS) {
        _width = BOSS_WIDTH - 1;
        _height = BOSS_HEIGHT;
        _speed = 12;
        _gunSpeed = 400;
        _hp = 750;
    }
}

int Enemy::GetEnemyType()
{
    return _type;
}

void Enemy::BossDir()
{
    if (_x + _width >= COLS - 4) {
        bossDir = false;
    }
    if (_x <= 4) {
        bossDir = true;
    }

    if (bossDir) _x++;
    else _x--;

    _gunType = rand() % 3;
}

// ------------------- BONUS -----------------

void Bonus::DrawObject()
{
    if (_type == GUNSPEED) {
        for (int h = 0; h < _height; h++)
        {
            for (int w = 0; w < _width; w++)
            {
                wData->vBuf[_y + h][_x + w] = gunspeedSprite[h][w] | (_color << 8);
            }
        }
    }
    else if (_type == GUNTYPE) {
        for (int h = 0; h < _height; h++)
        {
            for (int w = 0; w < _width; w++)
            {
                wData->vBuf[_y + h][_x + w] = guntypeSprite[h][w] | (_color << 8);
            }
        }
    }
    else if (_type == LIFE) {
        for (int h = 0; h < _height; h++)
        {
            for (int w = 0; w < _width; w++)
            {
                wData->vBuf[_y + h][_x + w] = lifeBonusSprite[h][w] | (_color << 8);
            }
        }
    }
    
}

void Bonus::EraseObject()
{
    for (int h = 0; h < _height; h++)
    {
        for (int w = 0; w < _width; w++)
        {
            wData->vBuf[_y + h][_x + w] = u' ';
        }
    }
}

void Bonus::MoveObject()
{
    EraseObject();

    _y++;
    if (_y + _height >= ROWS - 2) {
        DeleteObject();
    }
}

void Bonus::SetBonusType(int type)
{
    _type = type;

    if (_type == GUNSPEED) _color = BrPurple;
    else if (_type == GUNTYPE) _color = Yellow;
    else if (_type == LIFE) _color = Green;
}

int Bonus::GetBonusType()
{
    return _type;
}

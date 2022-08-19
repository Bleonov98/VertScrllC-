#include "Game.h"

void Game::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void Game::HotKeys(bool& pause)
{
	while (worldIsRun) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			worldIsRun = false;
		}
		else if (GetAsyncKeyState(0x50) & 0x8000) {
			pause = !pause;

			this_thread::sleep_for(milliseconds(1000));
		}
	}
}

void Game::DrawArea()
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");

	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

		if (hLoadedResource)
		{
			LPCSTR area = (LPCSTR)LockResource(hLoadedResource);

			if (area)
			{
				DWORD dwResourceSize = SizeofResource(hInstance, hResource);

				if (0 != dwResourceSize)
				{
					int row = 0, col = 0;
					for (int i = 0; i < strnlen(area, COLS * (ROWS * 20)); i++) {
						GameMap[row][col] = area[i];
						if (i % COLS == 0 && i > 0) {
							row++;
							col = 0;
						}
						col++;
					}
				}
			}
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
}

void Game::CreateWorld() {

	term.Terminal();  // Set virtual terminal settings
	term.SetScreenSize();
	term.SetConsoleFont();

	printf(CSI "?1049h"); // enable alt buffer
	printf(CSI "?25l"); // hide cursor blinking

	DrawArea();
}

void Game::DrawEndInfo(bool& restart)
{
	if (win) {
		PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);
		SetPos(COLS + 1, 20);
		cout << "CONGRATULATION! YOU WIN!";
	}
	else {
		PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);
		SetPos(COLS + 7, 20);
		cout << "GAME OVER!";
	}

	SetPos(COLS + 1, 23);
	cout << "PRESS ENTER TO RESTART";
	SetPos(COLS + 3, 24);
	cout << "PRESS ESC TO EXIT";

	bool pressed = false;
	restart = false;

	while (!pressed) {
		if (GetAsyncKeyState(VK_RETURN)) {
			restart = true;
			pressed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			restart = false;
			pressed = true;
		}
	}
}

void Game::DrawInfo()
{
	int _speed = 1;

	if (player->GetGunSpeed() == 500) _speed = 1;
	else if (player->GetGunSpeed() == 450) _speed = 2;
	else if (player->GetGunSpeed() == 400) _speed = 3;
	else if (player->GetGunSpeed() == 350) _speed = 4;
	else if (player->GetGunSpeed() == 300) _speed = 5;
	else if (player->GetGunSpeed() == 250) _speed = 6;
	else if (player->GetGunSpeed() == 200) _speed = 7;

	for (int i = 0; i < 8; i++)
	{
		SetPos(COLS + 5 + i, 5);
		printf(CSI "0m");
		cout << ' ';
	}

	if (player->GetHp() == 100) {
		SetPos(COLS + 1, 5);
		cout << "HP: ";
		printf(CSI "42;32m");
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		printf(CSI "0m");
	}
	else if (player->GetHp() == 75) {
		SetPos(COLS + 1, 5);
		cout << "HP: ";
		printf(CSI "43;32m");
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		printf(CSI "0m");
	}
	else if (player->GetHp() == 50) {
		SetPos(COLS + 1, 5);
		cout << "HP: ";
		printf(CSI "43;32m");
		cout << ' ';
		cout << ' ';
		cout << ' ';
		cout << ' ';
		printf(CSI "0m");
	}
	else if (player->GetHp() == 25) {
		SetPos(COLS + 1, 5);
		cout << "HP: ";
		printf(CSI "41;32m");
		cout << ' ';
		cout << ' ';
		printf(CSI "0m");
	}

	SetPos(COLS + 1, 7);
	cout << "LIFES: " << player->GetLifes();
	SetPos(COLS + 1, 9);
	cout << "SPEED: " << _speed;
	SetPos(COLS + 1, 15);
	cout << "SCORE: " << score;
	
}

void Game::DrawChanges()
{
	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			if (prevBuf[y][x] == wData.vBuf[y][x]) {
				continue;
			}
			else {
				prevBuf[y][x] = wData.vBuf[y][x];

				SetPos(x, y);

				if ((prevBuf[y][x] >> 8) == Red) {
					printf(CSI "22;31m");
				}
				else if ((prevBuf[y][x] >> 8) == Green) {
					printf(CSI "22;32m");
				}
				else if ((prevBuf[y][x] >> 8) == Blue) {
					printf(CSI "22;34m");
				}
				else if ((prevBuf[y][x] >> 8) == Yellow) {
					printf(CSI "22;33m");
				}
				else if ((prevBuf[y][x] >> 8) == Purple) {
					printf(CSI "22;35m");
				}
				else if ((prevBuf[y][x] >> 8) == Cyan) {
					printf(CSI "22;36m");
				}
				else if ((prevBuf[y][x] >> 8) == BrRed) {
					printf(CSI "22;91m");
				}
				else if ((prevBuf[y][x] >> 8) == BrGreen) {
					printf(CSI "22;92m");
				}
				else if ((prevBuf[y][x] >> 8) == BrBlue) {
					printf(CSI "22;94m");
				}
				else if ((prevBuf[y][x] >> 8) == BrCyan) {
					printf(CSI "22;96m");
				}
				else if ((prevBuf[y][x] >> 8) == BrPurple) {
					printf(CSI "22;95m");
				}
				else if ((prevBuf[y][x] >> 8) == BrYellow) {
					printf(CSI "22;93m");
				}
				else if ((prevBuf[y][x] >> 8) == White) {
					printf(CSI "47;37m");
				}
				else printf(CSI "22; 44m");

				cout << char(prevBuf[y][x]);

				printf(CSI "1;0m");
			}
		}
	}
}

void Game::SpawnEnemy(int x, int y, int type)
{
	enemy = new Enemy(&wData, x, y, 3, BrYellow);
	enemy->SetEnemyType(type);
	allObjectList.push_back(enemy);
	enemyList.push_back(enemy);
}

void Game::SpawnBonus(Enemy* enemy, int type)
{
	if (type >= 3) return;

	bonus = new Bonus(&wData, enemy->GetX() - 1, enemy->GetY(), 1, Yellow);

	bonus->SetBonusType(type);
	allObjectList.push_back(bonus);
	bonusList.push_back(bonus);
}

void Game::DrawToMem()
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->IsObjectDelete()) {
			enemyList.erase(enemyList.begin() + i);
			i = -1;
		}
	}

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->IsObjectDelete()) {
			bulletList.erase(bulletList.begin() + i);

			i = -1;
		}
	}

	for (int i = 0; i < bonusList.size(); i++)
	{
		if (bonusList[i]->IsObjectDelete()) {
			bonusList.erase(bonusList.begin() + i);

			i = -1;
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		if (allObjectList[i]->IsObjectDelete()) {
			wData.vBuf[allObjectList[i]->GetY()][allObjectList[i]->GetX()] = u' ';
			allObjectList.erase(allObjectList.begin() + i);

			i = -1;
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		allObjectList[i]->DrawObject();
	}
}

void Game::Shot(int owner, Character* character)
{
	if (character->GetGunType() == SHOT) {
		if (owner == PLAYER) {
			bullet = new Bullet(&wData, character->GetX() + character->GetWidth() / 2, character->GetY() - 1, 1, Red);
			bullet->SetOwner(owner);
			bulletList.push_back(bullet);
			allObjectList.push_back(bullet);
		}
		else if (owner == ENEMY) {
			bullet = new Bullet(&wData, character->GetX() + character->GetWidth() / 2, character->GetY() + character->GetHeight() + 1, 1, Red);
			bullet->SetOwner(owner);
			bulletList.push_back(bullet);
			allObjectList.push_back(bullet);
		}
	}
	else if (character->GetGunType() == DOUBLESHOT) {
		for (int i = 0; i < 2; i++)
		{
			if (i == 0) {
				if (owner == PLAYER) {
					bullet = new Bullet(&wData, character->GetX() + character->GetWidth() - 1, character->GetY() - 1, 1, Red);
					bullet->SetOwner(owner);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}
				else {
					bullet = new Bullet(&wData, character->GetX(), character->GetY() + character->GetHeight() + 1, 1, Red);
					bullet->SetOwner(owner);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}

			}
			else {
				if (owner == PLAYER) {
					bullet = new Bullet(&wData, character->GetX(), character->GetY() - 1, 1, Red);
					bullet->SetOwner(owner);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}
				else {
					bullet = new Bullet(&wData, character->GetX() + character->GetWidth() - 1, character->GetY() + character->GetHeight() + 1, 1, Red);
					bullet->SetOwner(owner);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}

			}
		}
	}
	else if (character->GetGunType() == ROCKET) {
		if (owner == PLAYER) {
			bullet = new Bullet(&wData, character->GetX() + character->GetWidth() / 2, character->GetY() - 1, 1, Red);
			bullet->SetOwner(owner);
			bullet->SetBulletType(character->GetGunType());
			if (enemyList.empty()) {
				bullet->RocketPath(45, 4);
			}
			else {
				bullet->RocketPath(enemyList[0]->GetX() + 1, enemyList[0]->GetY() + 1);
			}
			bulletList.push_back(bullet);
			allObjectList.push_back(bullet);
		}
		else {
			bullet = new Bullet(&wData, character->GetX() + character->GetWidth() / 2, character->GetY() - 1, 1, Red);
			bullet->SetOwner(owner);
			bullet->SetBulletType(character->GetGunType());
			bullet->RocketPath(player->GetX() + 1, player->GetY() + player->GetHeight());
			bulletList.push_back(bullet);
			allObjectList.push_back(bullet);
		}
	}

	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
}

void Game::Collision()
{
	for (int enemy = 0; enemy < enemyList.size(); enemy++)
	{

		bool finded = false;

		for (int h = 0; h < enemyList[enemy]->GetHeight(); h++)
		{
			for (int w = 0; w < enemyList[enemy]->GetWidth(); w++)
			{

				for (int ph = 0; ph < player->GetHeight(); ph++)
				{
					for (int pw = 0; pw < player->GetWidth(); pw++)
					{
						if ((enemyList[enemy]->GetX() + w == player->GetX() + pw) && (enemyList[enemy]->GetY() + h == player->GetY() + ph)) {

							enemyList[enemy]->DeleteObject();

							player->Death(worldIsRun);


							finded = true;
							break;
						}
					}
					if (finded) break;
				}
				if (finded) break;
			}
			if (finded) break;
		}

		finded = false;
	}

	for (int bullet = 0; bullet < bulletList.size(); bullet++)
	{

		bool finded = false;

		if (bulletList[bullet]->GetOwner() == PLAYER) {
			for (int enemy = 0; enemy < enemyList.size(); enemy++)
			{

				for (int h = 0; h < enemyList[enemy]->GetHeight(); h++)
				{
					for (int w = 0; w < enemyList[enemy]->GetWidth(); w++)
					{

						if (bulletList[bullet]->GetX() == enemyList[enemy]->GetX() + w &&
							bulletList[bullet]->GetY() == enemyList[enemy]->GetY() + h)
						{
							enemyList[enemy]->Hit(score);
							bulletList[bullet]->DeleteObject();

							SpawnBonus(enemyList[enemy], rand() % 7);

							finded = true;

							break;
						}

					}
					if (finded) break;
				}
				if (finded) break;

			}
		}
		else if (bulletList[bullet]->GetOwner() == ENEMY) {
			for (int h = 0; h < player->GetHeight(); h++)
			{
				for (int w = 0; w < player->GetWidth(); w++)
				{
					if (bulletList[bullet]->GetX() == player->GetX() + w && bulletList[bullet]->GetY() == player->GetY() + h) {
						player->Death(worldIsRun);

						bulletList[bullet]->DeleteObject();

						finded = true;
						break;
					}
				}
				if (finded) break;
			}
			if (finded) break;
		}
	}

	for (int bonus = 0; bonus < bonusList.size(); bonus++)
	{
		bool finded = false;
		for (int h = 0; h < bonusList[bonus]->GetHeight(); h++)
		{
			for (int w = 0; w < bonusList[bonus]->GetWidth(); w++)
			{

				for (int ph = 0; ph < player->GetHeight(); ph++)
				{
					for (int pw = 0; pw < player->GetWidth(); pw++)
					{

						if (bonusList[bonus]->GetX() + w == player->GetX() + pw &&
							bonusList[bonus]->GetY() + h == player->GetY() + ph) {

							bonusList[bonus]->DeleteObject();
							score += 150;

							PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC);

							if (bonusList[bonus]->GetBonusType() == GUNSPEED) {
								if (player->GetGunSpeed() > 300) {
									player->SetGunSpeed(player->GetGunSpeed() - 100);

									finded = true;
									break;
								}
								else {
									finded = true;
									break;
								}
							}
							else if (bonusList[bonus]->GetBonusType() == GUNTYPE) {
								if (player->GetGunType() == SHOT) player->SetGunType(DOUBLESHOT);
								else if (player->GetGunType() == DOUBLESHOT) player->SetGunType(ROCKET);

								finded = true;
								break;
							}
							else if (bonusList[bonus]->GetBonusType() == LIFE) {
								player->AddLifes();

								finded = true;
								break;
							}

						}
					}
					if (finded) break;
				}
				if (finded) break;
			}
			if (finded) break;
		}
		finded = false;
	}
}

void Game::WallCollision()
{
	for (int bullet = 0; bullet < bulletList.size(); bullet++)
	{
		if (wData.grid[bulletList[bullet]->GetY()][bulletList[bullet]->GetX()] == -1 ||
			wData.grid[bulletList[bullet]->GetY() - 1][bulletList[bullet]->GetX()] == -1 ||
			wData.grid[bulletList[bullet]->GetY() + 1][bulletList[bullet]->GetX()] == -1) {
			bulletList[bullet]->DeleteObject();

			break;
		}
	}
	for (int enemy = 0; enemy < enemyList.size(); enemy++)
	{
		bool finded = false;
		for (int eh = 0; eh < enemyList[enemy]->GetHeight() - 1; eh++)
		{
			for (int ew = 0; ew < enemyList[enemy]->GetWidth(); ew++)
			{
				if (wData.grid[enemyList[enemy]->GetY() + eh][enemyList[enemy]->GetX() + ew] == -1) {
					enemyList[enemy]->DeleteObject();

					finded = true;

					break;
				}
			}
			if (finded) break;
		}
		finded = false;
	}
	for (int bonus = 0; bonus < bonusList.size(); bonus++)
	{
		bool finded = false;
		for (int bh = 0; bh < bonusList[bonus]->GetHeight() - 1; bh++)
		{
			for (int bw = 0; bw < bonusList[bonus]->GetWidth(); bw++)
			{
				if (wData.grid[bonusList[bonus]->GetY() + bh][bonusList[bonus]->GetX() + bw] == -1) {
					bonusList[bonus]->DeleteObject();

					finded = true;

					break;
				}
			}
			if (finded) break;
		}
		finded = false;
	}

	for (int ph = 0; ph < player->GetHeight() - 1; ph++)
	{
		bool finded = false;
		for (int pw = 0; pw < player->GetWidth(); pw++)
		{
			if (wData.grid[player->GetY() + ph][player->GetX() + pw] == -1) {
				player->EraseObject();

				player->Death(worldIsRun);

				player->SetX(COLS / 2);
				player->SetY(ROWS - 4);

				finded = true;

				break;
			}
		}
		if (finded) break;
	}
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	bool pause = false;
	score = 0;

	thread hotKeys([&]
		{ HotKeys(pause); }
	);

	int scrollY = ROWS * 20, scrollSpeed = 2, tick = 0;

	player = new Player(&wData, COLS / 2, ROWS - 15, 2, BrBlue);
	allObjectList.push_back(player);

	while (worldIsRun) {

		if (pause) {

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "PAUSED";

			while (pause) {

			}

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "      ";

		}

		if (tick % player->GetSpeed() == 0) {
			player->MoveObject();
			if (GetAsyncKeyState(VK_SPACE) && player->GetGunState()) {
				Shot(PLAYER, player);
				thread reloadGun([&]
					{ player->ReloadGun(); }
				);
				reloadGun.detach();
			}
		}
		
		for (int i = 0; i < enemyList.size(); i++)
		{
			if (tick % enemyList[i]->GetSpeed() == 0) {
				enemyList[i]->MoveObject();

				if (enemyList[i]->GetGunState() && tick % 10 == 0) {
					enemy = enemyList[i];
					Shot(ENEMY, enemy);
					thread reloadGun([&]
						{ enemy->ReloadGun(); }
					);
					reloadGun.detach();
				}

			}
		}

		for (int i = 0; i < bulletList.size(); i++)
		{
			if (tick % bulletList[i]->GetSpeed() == 0) {
				bulletList[i]->MoveObject();
			}
		}
		
		for (int i = 0; i < bonusList.size(); i++)
		{
			if (tick % bonusList[i]->GetSpeed() == 0) {
				bonusList[i]->MoveObject();
			}
		}

		Collision();

		WallCollision();

		DrawToMem();

		DrawChanges();

		if (tick % scrollSpeed == 0 && tick > 0) ScrollWindow(scrollY);

		DrawInfo();

		Sleep(15);

		tick++;

		if (scrollY - ROWS == 0 && enemyList.empty()) {
			worldIsRun = false;
			win = true;
		}
	}

	DrawEndInfo(restart);

	hotKeys.join();

	printf(CSI "?1049l");
}
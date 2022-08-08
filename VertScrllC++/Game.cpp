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
		SetPos(44, ROWS + 2);
		cout << "CONGRATULATION! YOU WIN!";
	}
	else {
		SetPos(50, ROWS + 2);
		cout << "GAME OVER!";
	}

	SetPos(45, ROWS + 3);
	cout << "PRESS ENTER TO RESTART";
	SetPos(47, ROWS + 4);
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
	enemy = new Enemy(&wData, x, y, 2, BrYellow);
	enemy->SetEnemyType(type);
	allObjectList.push_back(enemy);
	enemyList.push_back(enemy);
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

	//for (int i = 0; i < bonusList.size(); i++)
	//{
	//	if (bonusList[i]->IsObjectDelete()) {
	//		bonusList.erase(bonusList.begin() + i);

	//		i = -1;
	//	}
	//}

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
					bullet = new Bullet(&wData, character->GetX() + character->GetWidth(), character->GetY() + character->GetHeight() + 1, 1, Red);
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
			bullet->RocketPath(15, 10);
			bulletList.push_back(bullet);
			allObjectList.push_back(bullet);
		}
		else {
			bullet = new Bullet(&wData, character->GetX() + character->GetWidth() / 2, character->GetY() - 1, 1, Red);
			bullet->SetOwner(owner);
			bullet->SetBulletType(character->GetGunType());
			bullet->RocketPath(player->GetX(), player->GetY());
			bulletList.push_back(bullet);
			allObjectList.push_back(bullet);
		}
	}
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
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

	SpawnEnemy(5, 5, SMALL);

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

				if (enemyList[i]->GetGunState()) {
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
		
		

		//Collision(player);

		//WallCollision(player);

		DrawToMem();

		DrawChanges();

		if (tick % scrollSpeed == 0 && tick > 0) ScrollWindow(scrollY);

		//DrawInfo(player);

		Sleep(20);

		tick++;
	}

	DrawEndInfo(restart);

	hotKeys.join();

	printf(CSI "?1049l");
}
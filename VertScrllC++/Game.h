#include "GameObject.h"


class Game
{
private:
    wd wData;

    char16_t prevBuf[ROWS][COLS];
    char coord[100];

    unsigned char GameMap[ROWS*20][COLS];

    unsigned char prevActiveAreaBuf[ROWS][COLS];
    unsigned char activeAreaBuf[ROWS][COLS];

    bool worldIsRun = true, win = false;

    int score = 0;

    vector<GameObject*> allObjectList;
    vector<Enemy*> enemyList;
    vector<Bullet*> bulletList;

    Player* player;
    Bullet* bullet;
    Enemy* enemy;


    HINSTANCE hInstance;

    class VirtualTerminal {
    public:

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO csbi;

        _SMALL_RECT Rect;

        HWND hWindowConsole = GetForegroundWindow();

        int Width = 91, Height = 56, err = 30;

        bool Terminal() {

            // Set output mode to handle virtual terminal sequences
            if (hOut == INVALID_HANDLE_VALUE)
            {
                return false;
            }
            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
            if (hIn == INVALID_HANDLE_VALUE)
            {
                return false;
            }

            DWORD dwOriginalOutMode = 0;
            DWORD dwOriginalInMode = 0;
            if (!GetConsoleMode(hOut, &dwOriginalOutMode))
            {
                return false;
            }
            if (!GetConsoleMode(hIn, &dwOriginalInMode))
            {
                return false;
            }

            DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
            DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

            DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
            if (!SetConsoleMode(hOut, dwOutMode))
            {
                // we failed to set both modes, try to step down mode gracefully.
                dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
                if (!SetConsoleMode(hOut, dwOutMode))
                {
                    // Failed to set any VT mode, can't do anything here.
                    return -1;
                }
            }

            DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
            if (!SetConsoleMode(hIn, dwInMode))
            {
                // Failed to set VT input mode, can't do anything here.
                return -1;
            }

            return 0;
        }

        void SetScreenSize() {
            COORD monitorSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
            COORD bufferSize = { Width, Height };

            _SMALL_RECT Rect;
            Rect.Top = 0;
            Rect.Left = 0;
            Rect.Bottom = Height - 1;
            Rect.Right = Width - 1;

            SetConsoleScreenBufferSize(hOut, bufferSize);

            SetWindowPos(hWindowConsole, HWND_NOTOPMOST,
                (monitorSize.X / 2 - Width * 4 - err * 4), (monitorSize.Y / 2 - Height * 8 - err), 0, 0,
                SWP_NOZORDER | SWP_NOREPOSITION | SWP_NOREDRAW);

            SetConsoleWindowInfo(hOut, TRUE, &Rect);

            SetWindowLong(hWindowConsole, GWL_STYLE, GetWindowLong(hWindowConsole, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

            SetConsoleTitleA("ScrollerTypeGame C++");
        }

        void SetConsoleFont() {

            CONSOLE_FONT_INFOEX cfi;

            cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
            cfi.nFont = 6;
            cfi.dwFontSize.X = 30;
            cfi.dwFontSize.Y = 13;
            cfi.FontFamily = 54;
            cfi.FontWeight = 1000;

            wcscpy_s(cfi.FaceName, L"Lucida Console");
            SetCurrentConsoleFontEx(hOut, 0, &cfi);
        }
    };

    void ScrollWindow(int& scrollY) {

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                wData.grid[i][j] = 0;
            }
        }


        int activeY = ROWS - 1;
        for (int i = scrollY - 1; i >= scrollY - ROWS; i--)
        {
            for (int j = 0; j < COLS; j++)
            {
                activeAreaBuf[activeY][j] = GameMap[i][j];

                if (activeAreaBuf[activeY][j] == '#') {
                    wData.grid[activeY][j] = -1;
                }
            }
            activeY--;
        }

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if ((prevActiveAreaBuf[i][j] != activeAreaBuf[i][j]) && (wData.vBuf[i][j + 1] == 0 || wData.vBuf[i][j + 1] == u' '))
                {
                    prevActiveAreaBuf[i][j] = activeAreaBuf[i][j];

                    SetPos(j + 1, i);

                    cout << prevActiveAreaBuf[i][j];
                }
            }
        }

        if (scrollY - ROWS > 0) scrollY--; 

        /* if (scrollX % 120 == 0 && scrollX > 0) {
            SpawnEnemy(COLS - 10, 3 + rand() % (ROWS - 6), REGULAR);
        }
        if (scrollX % 50 == 0 && scrollX > 0) {
            SpawnEnemy(COLS - 10, 3 + rand() % (ROWS - 6), SMALL);
        }
        if (scrollX % 200 == 0 && scrollX > 0) {
            SpawnEnemy(COLS - 10, ROWS - 3, LAND);
        }
        if (scrollX + COLS == 1020) {
            SpawnEnemy(COLS - 10, 3 + rand() % (ROWS - 6), BOSS);
            PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);
        }*/
    }

    VirtualTerminal term; // console setting

protected:

    void SetPos(int x, int y);

    void DrawArea();

    void CreateWorld();

    void DrawToMem();

    void HotKeys(bool& pause);

    void DrawEndInfo(bool& restart);

    void DrawInfo();

    void DrawChanges();

    void SpawnEnemy(int x, int y, int type);

    //void SpawnBonus(Enemy* enemy, int type);

    void Shot(int owner, Character* character);

    //void Collision(Player* player);

    //void WallCollision(Player* player);

public:

    void RunWorld(bool& restart);

};

#include "BattleGrid.h"
#include "BattleGrid.cpp"
#define IDC_BUTTON_RANDOM   01
#define IDC_START_GAME      02
#define FRIEND_GRID_X       20
#define FRIEND_GRID_Y       20
#define ENEMY_GRID_X        480
#define ENEMY_GRID_Y        20

#define SAMPLING            40

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "CaptainBumbu";

BattleGrid friendGrid(FRIEND_GRID_X, FRIEND_GRID_Y, SAMPLING, false);
BattleGrid enemyGrid(ENEMY_GRID_X, ENEMY_GRID_Y, SAMPLING, true);
HBITMAP hatch, miss, dead;

bool startGame = false;
HINSTANCE hInst;
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    hInst = hThisInstance;

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(255, 255, 255));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
             0,                   /* Extended possibilites for variation */
             szClassName,         /* Classname */
             "Code::Blocks Template Windows App",       /* Title Text */
             WS_OVERLAPPEDWINDOW, /* default window */
             CW_USEDEFAULT,       /* Windows decides the position */
             CW_USEDEFAULT,       /* where the window ends up on the screen */
             1000,                 /* The programs width */
             600,                 /* and height in pixels */
             HWND_DESKTOP,        /* The window is a child-window to desktop */
             NULL,                /* No menu */
             hThisInstance,       /* Program Instance handler */
             NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    friendGrid.shuffleShip();
    enemyGrid.shuffleShip();


    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    HWND hBtnRandom, hBtnStartGame;
    hdc = GetDC (hwnd);


    bool friendHit = false;
    if (friendGrid._attackResult != BattleGrid::ATTACK_MISS) {
        while (friendGrid._attackResult != BattleGrid::ATTACK_MISS) {
            friendGrid.autoAttack();
            if (friendGrid._attackResult != BattleGrid::ATTACK_NONE)
                friendHit = true;
            if (friendGrid._attackResult == BattleGrid::ATTACK_MISS)
                enemyGrid._attackResult = BattleGrid::ATTACK_SUCCESS;
        }
    } else
        enemyGrid._attackResult = BattleGrid::ATTACK_SUCCESS;

    if (friendHit)
        friendGrid.invalidateGrid(hwnd, hdc);

    switch (message)                  /* handle the messages */
    {

    case WM_CREATE: {
            hatch = (HBITMAP)LoadImage(hInst, "../bmp/sexy hasura.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            dead  = (HBITMAP)LoadImage(hInst, "../bmp/x smexy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            miss  = (HBITMAP)LoadImage(hInst, "../bmp/punctisor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            hBtnRandom = CreateWindowEx(NULL,
                            TEXT("button"),
                            "Random",
                            WS_TABSTOP | WS_VISIBLE |
                            WS_CHILD | BS_DEFPUSHBUTTON | BS_TOP,
                            30, 450,
                            70, 25,
                            hwnd,
                            (HMENU)IDC_BUTTON_RANDOM,
                            hInst,
                            NULL);

            hBtnStartGame = CreateWindowEx(NULL,
                            TEXT("button"),
                            "Start",
                            WS_TABSTOP | WS_VISIBLE |
                            WS_CHILD | BS_DEFPUSHBUTTON | BS_TOP,
                            150, 450,
                            70, 25,
                            hwnd,
                            (HMENU)IDC_START_GAME,
                            hInst,
                            NULL);
        }
        break;

    case WM_PAINT: {
            HDC paintHdc = BeginPaint(hwnd, &ps);

            friendGrid.drawGrid(paintHdc);
            friendGrid.drawLivingShips(paintHdc, hatch);
            friendGrid.drawDamage(paintHdc, dead, miss);

            enemyGrid.drawGrid(paintHdc);
            enemyGrid.drawDamage(paintHdc, dead, miss);


            EndPaint(hwnd, &ps);
            //ReleaseDC(hwnd, paintHdc);
        }
        break;
    /*case WM_ERASEBKGND:
        return 1;*/

    case WM_COMMAND: {
            switch (LOWORD(wParam)) {
            case IDC_BUTTON_RANDOM:
                if (!startGame) {
                    friendGrid.shuffleShip();
                    friendGrid.invalidateGrid(hwnd, hdc);
                }
                break;
            case IDC_START_GAME:
                startGame = true;
                enemyGrid._attackResult = BattleGrid::ATTACK_SUCCESS;
                friendGrid._attackResult = BattleGrid::ATTACK_MISS;
                break;
            }
        }
        break;
    case WM_LBUTTONUP: {
            if (startGame) {
                int xPos = LOWORD (lParam);
                int yPos = HIWORD (lParam);
                if (enemyGrid._attackResult == BattleGrid::ATTACK_NONE || enemyGrid._attackResult == BattleGrid::ATTACK_SUCCESS) {
                    int result = enemyGrid.attackByCoords(xPos, yPos);
                    if (result == BattleGrid::ATTACK_MISS)
                        friendGrid._attackResult = BattleGrid::ATTACK_SUCCESS;

                    if (result != BattleGrid::ATTACK_NONE)
                        enemyGrid.invalidateGrid(hwnd, hdc);
                } else {
                    friendGrid._attackResult = BattleGrid::ATTACK_SUCCESS;
                }
            }
        }
        break;

    case WM_DESTROY:
        ReleaseDC(hwnd, hdc);
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        ReleaseDC(hwnd, hdc);
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    ReleaseDC(hwnd, hdc);
    return 0;
}

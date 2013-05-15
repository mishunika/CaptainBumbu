#include "BattleGrid.h"
#define FRIEND_GRID_X   20
#define FRIEND_GRID_Y   20
#define ENEMY_GRID_X    480
#define ENEMY_GRID_Y    20

#define SAMPLING        40

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "CaptainBumbu";

BattleGrid friendGrid(FRIEND_GRID_X, FRIEND_GRID_Y, SAMPLING);
BattleGrid enemyGrid(ENEMY_GRID_X, ENEMY_GRID_Y, SAMPLING);

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

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
    hdc = GetDC (hwnd);

    switch (message)                  /* handle the messages */
    {
    case WM_PAINT: {
            HDC paintHdc = BeginPaint(hwnd, &ps);
            friendGrid.drawGrid(paintHdc);
            enemyGrid.drawGrid(paintHdc);

            EndPaint(hwnd, &ps);
        }
        break;
    case WM_ERASEBKGND:
        return 1;

    case WM_DESTROY:
        ReleaseDC(hwnd, hdc);
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        ReleaseDC(hwnd, hdc);
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

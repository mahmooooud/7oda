#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

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
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

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
int Round(double x)
{
    return (int)(x+0.5);
}
void DrawLine(HDC hdc,int x1,int y1,int x2, int y2, COLORREF C)
{
    for(double t = 0 ; t<=1 ; t+=0.001 )
    {
        int x=Round(x1+(t*(x2-x1)));
        int y=Round(y1+(t*(y2-y1)));
        SetPixel(hdc,x,y,C);
    }
}

void DrawLineDDA(HDC hdc , int x1,int y1,int x2,int y2,COLORREF C)
{
    int x =x1,y= y1;
int dx = x2-x1;
int dy = y2-y1;
int e = 2*dy-dx;
int e1 = 2*(dy-dx);
int e2 = 2*dy;
SetPixel(hdc,x,y,C);
while(x<x2){
    x++;
    if(e>=0) {y++; e+=e1;}
    else e+=e2;
    SetPixel(hdc,x,y,C);
}
}

void MidPointLine(HDC hdc, int x1,int y1,int x2,int y2,COLORREF C){
int x=x1,y=y1,dx=x2-x1,dy=y2-y1;
int d = dx + (2*dy+1-2*y1) - 2*dy*(x1+1-x1);
SetPixel(hdc,x1,y1,C);
while(x<x2){
    if(d>0){
        x++;
        d+=(-2*dy);
    }
    else{
        x++;
        y++;
        d+=(-2*dy)+(2*dx);
    }
    SetPixel(hdc,x,y,C);
}
}
/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
      HDC hdc;
    static int x1, y1, x2, y2;
    switch (message)                  /* handle the messages */
    {
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_LBUTTONDOWN:
        x1 = LOWORD(lParam);
        y1 = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        x2 = LOWORD(lParam);
        y2 = HIWORD(lParam);
        hdc = GetDC(hwnd);
        DrawLine(hdc,x1,y1,x2,y2,RGB(200, 20, 20));
        ReleaseDC(hwnd, hdc);
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

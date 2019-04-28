#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>

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
void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc - y, yc - x, c);
    SetPixel(hdc, xc + y, yc - x, c);
}

void Ellipsee(HDC hdc,int xc,int yc, int R1,int R2,COLORREF c)
{
    int x =R1 ,y =R2;
    for(double t =0 ; t<=2 ; t+=0.001)
    {
      //x = round(t*sqrt((pow(R1*R2,2.0)-pow(R2*x,2))/pow(R1,2)));
    //y = round(t*sqrt((pow(R1*R2,2)-pow(R1*y,2))/pow(R2,2)));
      x = R1*cos(t);
      y = R2*sin(t);

    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + x, yc - y, c);
  //SetPixel(hdc, xc + y, yc + x, c);
//    SetPixel(hdc, xc - y, yc + x, c);
   // SetPixel(hdc, xc - y, yc - x, c);
   // SetPixel(hdc, xc + y, yc - x, c);

    }
}


void DrawEllipsePolar(HDC hdc,int xc,int yc, int R1,int R2,COLORREF c)
{
    int x ,y;
    double theta = 0;
    while(theta <360)
    {
        x = R1*cos(theta) + xc;
        y = R2*sin(theta) +yc;
        theta+=0.01;
        //Draw8Points(hdc,xc,yc,x,y,c);
        SetPixel(hdc,x,y,c);
    }

}

void DrawEllipse(HDC hdc,int xc,int yc, int R1,int R2,COLORREF c)
{
    int x = 0, y = R2;

    int p = pow(R2,2.0)- pow(R1,2)*R2 + pow(R1,2)/4;


    while(2.0*R2*R2*x <= 2.0*R1*R1*y)
    {
        if(p < 0)
        {
            x++;
            p = p+2*R2*R2*x+R2*R2;
        }
        else
        {
            x++;
            y--;
            p = p+2*R2*R2*x-2*R1*R1*y-R2*R2;
        }
        SetPixel(hdc, xc + x, yc + y, c);
        SetPixel(hdc, xc - x, yc + y, c);
        SetPixel(hdc, xc - x, yc - y, c);
        SetPixel(hdc, xc + x, yc - y, c);
    }

    p=R2*R2*(x+0.5)*(x+0.5)+R1*R1*(y-1)*(y-1)-R1*R1*R2*R2;
    while(y > 0)
    {
        if(p <= 0)
        {
            x++;
            y--;
            p = p+2*R2*R2*x-2*R1*R1*y+R1*R1;
        }
        else
        {
            y--;
            p = p-2*R1*R1*y+R1*R1;
        }
        SetPixel(hdc, xc + x, yc + y, c);
        SetPixel(hdc, xc - x, yc + y, c);
        SetPixel(hdc, xc - x, yc - y, c);
        SetPixel(hdc, xc + x, yc - y, c);
    }
}
/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HDC hdc;
    static int x1, y1, x2, y2, r1,r2;
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
        r2 = (int)sqrt(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
        r1 = r2*2/4;
        hdc = GetDC(hwnd);
        DrawEllipse(hdc, x1, y1, r1,r2, RGB(200, 20, 20));
        ReleaseDC(hwnd, hdc);
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }


    return 0;
}

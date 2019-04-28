#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<bits/stdc++.h>
using namespace std;
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
void drawline(HDC hdc, int x1,int y1,int x2, int y2,COLORREF c)
{

    for(double t = 0; t <= 1; t += 0.001)
    {
        int x=Round(x1+(t*(x2-x1)));
        int y=Round(y1+(t*(y2-y1)));
        //printf("X = %d, Y = %d\n", x, y);
        SetPixel(hdc,x,y,c);
    }

    /*
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
    */
}
//line clipping

union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    if(y>ytop)
        out.top=1;
    else if(y<ybottom)
        out.bottom=1;
    return out;
}
pair<int, int> VIntersect(double xs,double ys,double xe,double ye,int x,double &xi,double &yi)
{
    pair<int,int> p;
    xi=x;
    yi=ys+(x-xs)*(ye-ys)/(xe-xs);
    p.first=xi;
    p.second=yi;
    return p;
}
pair<int, int> HIntersect(double xs,double ys,double xe,double ye,int y,double &xi,double &yi)
{
    pair<int,int> p;
    yi=y;
    xi=xs+(y-ys)*(xe-xs)/(ye-ys);
    p.first=xi;
    p.second=yi;
    return p;
}
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom,COLORREF C)
{
//    drawline(hdc, xleft, ybottom, xleft, ytop, C);
//    drawline(hdc, xleft, ybottom, xright, ybottom, C);
//    drawline(hdc, xleft, ytop, xright, ytop, C);
//    drawline(hdc, xright, ybottom, xright, ytop, C);
    double x1=xs,y1=ys,x2=xe,y2=ye;

    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);

    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);

    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        pair<int, int> point;
        double xi,yi;

        if(out1.All)
        {
            if(out1.left)
                point = VIntersect(x1,y1,x2,y2,xleft, xi, yi);
            else if(out1.top)
                point = HIntersect(x1,y1,x2,y2,ytop, xi, yi);
            else if(out1.right)
                point = VIntersect(x1,y1,x2,y2,xright, xi, yi);
            else
                point = HIntersect(x1,y1,x2,y2,ybottom, xi, yi);
            x1=point.first;
            y1=point.second;
             out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
             }
        else
        {
            if(out2.left)
                point = VIntersect(x1,y1,x2,y2,xleft,xi,yi);
            else if(out2.top)
                point = HIntersect(x1,y1,x2,y2,ytop,xi,yi);
            else if(out2.right)
                point = VIntersect(x1,y1,x2,y2,xright,xi,yi);
            else
                point = HIntersect(x1,y1,x2,y2,ybottom,xi,yi);
            x2=point.first;
            y2=point.second;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }

    if(!out1.All && !out2.All)
    {
        /*MoveToEx(hdc,Round(x1),Round(y1),NULL);
        LineTo(hdc,Round(x2),Round(y2));*/
        drawline(hdc,x1,y1,x2,y2,C);


    }

}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int counter = 0;
    static int xW1, yW1, xW2, yW2;
    switch (message)                  /* handle the messages */
    {
        HDC hdc;
        static int x1,y1,x2,y2, x3, y3;

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_LBUTTONDOWN:
        if (counter != 0) break;
        xW1 = LOWORD(lParam);
        yW1 = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        if (counter != 0) break;
        xW2 = LOWORD(lParam);
        yW2 = HIWORD(lParam);
        counter = 1;
        hdc = GetDC(hwnd);

        drawline(hdc, xW1, yW1, xW1, yW2, COLORREF(RGB(255, 0, 0)));
        drawline(hdc, xW1, yW1, xW2, yW1, COLORREF(RGB(255, 0, 0)));
        drawline(hdc, xW1, yW2, xW2, yW2, COLORREF(RGB(255, 0, 0)));
        drawline(hdc, xW2, yW1, xW2, yW2, COLORREF(RGB(255, 0, 0)));

        ReleaseDC(hwnd, hdc);

        break;
    case WM_LBUTTONDBLCLK:
        if(counter == 1)
        {
            x1 = LOWORD(lParam);
            y1 = HIWORD(lParam);
            counter++;
        }
        else if(counter == 2)
        {   x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            counter++;
        }
        else if(counter == 3)
        {
            x3 = LOWORD(lParam);
            y3 = HIWORD(lParam);
            counter++;
        }
        //printf("Counter = %d\n", counter);
        if(counter >= 4)
        {
            hdc = GetDC(hwnd);
            //printf("IN DRAW\n");
            //drawline(hdc, x1, y1, x2, y2, COLORREF(RGB(255,0,255)));
            //drawline(hdc, x1, y1, x3, y3, COLORREF(RGB(0,0,0)));
            //drawline(hdc, x3, y3, x2, y2, COLORREF(RGB(0,0,255)));
            CohenSuth(hdc,x1,y1,x2,y2,xW1,yW2,xW2,yW1,RGB(255,100,255));
            CohenSuth(hdc,x1, y1, x3, y3,xW1,yW2,xW2,yW1,RGB(255,100,255));
            CohenSuth(hdc,x3, y3, x2, y2,xW1,yW2,xW2,yW1,RGB(255,100,155));
            counter = 1;
            ReleaseDC(hwnd,hdc);
        }
        //hdc = GetDC(hwnd);
//        drawline(hdc,50,500,500,500,RGB(255,0,0));
//        drawline(hdc,500,500,500,50,RGB(255,0,0));
//        drawline(hdc,500,50,50,50,RGB(255,0,0));
//        drawline(hdc,50,50,50,500,RGB(255,0,0));
        //drawline(hdc,50,500,500,500,RGB(255,0,0));
        //ReleaseDC(hwnd,hdc);
        //integerdda(hdc,x1,y1,x2,y2,RGB(255,0,0));

        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

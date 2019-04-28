#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <bits/stdc++.h>
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

struct vertex
{

    double x, y;
    vertex(double x1=0 , double y1=0)
    {
        x = x1;
        y = y1;
    }
};

// clippingList;
//bool Is_inside(vertex &v,int edge);
typedef bool (*Is_inside)(vertex& v,int edge);
typedef vertex (*Intersect)(vertex& v1,vertex& v2,int edge);
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

vector<vertex> ClipWithEdge(vector<vertex> p,int edge,Is_inside inside ,Intersect intersection )
{
    vector<vertex> output;
    vertex v = p[p.size()-1];
    bool v_inside = inside(v,edge);

    for(int i=0; i<(int)p.size(); i++)
    {
        vertex v2=p[i];
        bool v2_in=inside(v2,edge);
        if(!v_inside && v2_in)
        {
            output.push_back(intersection(v,v2,edge));
            output.push_back(v2);
        }
        else if(v_inside && v2_in) output.push_back(v2);
        else if(v_inside) output.push_back(intersection(v,v2,edge));
        v=v2;
        v_inside=v2_in;

    }
    return output;

}
bool InLeft(vertex& v,int edge)
{
    return v.x>=edge;
}
bool InRight(vertex& v,int edge)
{
    return v.x<=edge;
}
bool InTop(vertex& v,int edge)
{
    return v.y>=edge;
}
bool InBottom(vertex& v,int edge)
{
    return v.y<=edge;
}

vertex VIntersect(vertex& v1,vertex& v2,int xedge)
{
    vertex res;
    res.x=xedge;
    res.y=v1.y+(xedge-v1.x)*(v2.y-v1.y)/(v2.x-v1.x);
    return res;
}

vertex HIntersect(vertex& v1,vertex& v2,int yedge)
{
    vertex res;
    res.y=yedge;
    res.x=v1.x+(yedge-v1.y)*(v2.x-v1.x)/(v2.y-v1.y);
    return res;
}

void PolygonClip(HDC hdc,POINT *p,int n,int xleft,int ytop,int xright,int ybottom)
{
    vector<vertex> vlist;
    for(int i=0; i<n; i++)vlist.push_back(vertex(p[i].x,p[i].y));
    vlist=ClipWithEdge(vlist,xleft,InLeft,VIntersect);
    vlist=ClipWithEdge(vlist,ytop,InTop,HIntersect);
    vlist=ClipWithEdge(vlist,xright,InRight,VIntersect);
    vlist=ClipWithEdge(vlist,ybottom,InBottom,HIntersect);
    vertex v1=vlist[vlist.size()-1];
    for(int i=0; i<(int)vlist.size(); i++)
    {

        vertex v2=vlist[i];
        drawline(hdc,v1.x,v1.y,v2.x,v2.y,RGB(128,0,128));
        v1=v2;
    }
}
struct point{
int x,y;};

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int counter = 0;
    static int xW1, yW1, xW2, yW2;
    point v1,v2,v3,v4,v5;
  static  POINT *p = new POINT[5];
    switch (message)                  /* handle the messages */
    {
        HDC hdc;
        static int x1,y1,x2,y2, x3, y3,x4,x5,y4,y5;

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
                p[0].x = LOWORD(lParam);
                p[0].y = HIWORD(lParam);
                ++counter;
                std::cout<<"\ncounter => "<<counter<<"   "<<p[0].x<<"  "<<p[0].y;

        }
        else if(counter == 2)
        {
    p[1].x = LOWORD(lParam);
                p[1].y = HIWORD(lParam);
                ++counter;
                std::cout<<"\ncounter => "<<counter<<"   "<<p[1].x<<"  "<<p[1].y;
        }
        else if(counter == 3)
        {
              p[2].x = LOWORD(lParam);
                p[2].y = HIWORD(lParam);
                ++counter;
                std::cout<<"\ncounter => "<<counter<<"   "<<p[2].x<<"  "<<p[2].y;
        }

        else if(counter == 4)
        {
             p[3].x = LOWORD(lParam);
                p[3].y = HIWORD(lParam);
                ++counter;
                std::cout<<"\ncounter => "<<counter<<"   "<<p[3].x<<"  "<<p[3].y;
        }

        else if(counter == 5)
        {
              p[4].x = LOWORD(lParam);
                p[4].y = HIWORD(lParam);
                ++counter;
                std::cout<<"\ncounter => "<<counter<<"   "<<p[4].x<<"  "<<p[4].y<<endl<<"\n\n";

        }

        if(counter >= 6)
        {
            hdc = GetDC(hwnd);
           for(int i = 0 ; i <5 ; i++)
               cout<<"point : "<<p[i].x<<"  "<<p[i].y<<endl;

          /*  drawline(hdc,x1,y1,x2,y2,RGB(255,69,0));
            drawline(hdc,x2,y2,x3,y3,RGB(255,140,0));
            drawline(hdc,x3,y3,x4,y4,RGB(255,165,0));
            drawline(hdc,x4,y4,x5,y5,RGB(139,137,137));
            drawline(hdc,x5,y5,x1,y1,RGB(128,0,128));
            */
            PolygonClip(hdc,p,5,xW1,yW1,xW2,yW2);
            counter = 1;
            ReleaseDC(hwnd,hdc);
        }


        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }


    return 0;
}

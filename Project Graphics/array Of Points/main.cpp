#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
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

struct points{
int x ;
int y ;
};
/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static points *p = new points[3];
    static int counter = 0;
    static int xW1, yW1, xW2, yW2;
    HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
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
            if(counter == 0 ){
                p[0].x = LOWORD(lParam);
                p[0].y = HIWORD(lParam);
                counter++;
                   std::cout<<"\ncounter => "<<counter<<"   "<<p[0].x<<"  "<<p[0].y;
            }
            else if(counter == 1 ){
                p[1].x = LOWORD(lParam);
                p[1].y = HIWORD(lParam);
                ++counter;
                   std::cout<<"\ncounter => "<<counter<<"   "<<p[1].x<<"  "<<p[1].y;
            }
            else if(counter == 2 ){
                p[2].x = LOWORD(lParam);
                p[2].y = HIWORD(lParam);
                ++counter;
                std::cout<<"\ncounter => "<<counter<<"   "<<p[2].x<<"  "<<p[2].y;
            }
            if(counter >= 3){
                    std::cout<<"\n After ";
                for(int i=0 ; i< 3 ; i++){
                    std::cout<<p[i].x<<"   "<<p[i].y<<"\n";

                }
                counter =0;
            }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

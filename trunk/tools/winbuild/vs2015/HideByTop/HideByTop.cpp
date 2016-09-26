// HideByTop.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HideByTop.h"

#include <cstdio>
#include <iostream>

#include <io.h>  
#include <fcntl.h>  
#include <iostream>  

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	{
		AllocConsole();                      // 为一个进程定位一个console ，如果是win32 程序的话这里就派上用场了 
											 //Retrieves a handle to the specified standard device (standard input, standard output, or standard error). 
		HANDLE hin = ::GetStdHandle(STD_INPUT_HANDLE);
		HANDLE hout = ::GetStdHandle(STD_OUTPUT_HANDLE);
		//Associates a C run-time file descriptor with an existing operating-system file handle. 
		int hcin = _open_osfhandle((intptr_t)hin, _O_TEXT);                 // 此时hcin 就成了一个file descriptor 了 
																			//		When a program opens a file, the operating system returns a corresponding file descriptor that the program refers to 
																			//     in order to process the file. A file descriptor is a low positive integer. The first three file descriptors (0,1, and 2,) 
																			//     are associated with the standard input (stdin), the standard output (stdout), and the standard error (stderr), respectively. 
																			//     Thus, the function scanf() uses stdin and the function printf() uses stdout. You can override the default setting and 
																			//     re-direct the process's I/O to different files by using different file descriptors: 
																			//     #include <cstdio> 
																			//     fprintf(stdout, "writing to stdout"); //write to stdout instead of a physical file 
		FILE * fpin = _fdopen(hcin, "r");
		*stdin = *fpin;                                                  //stdin 就指向了文件指针 
		int hcout = _open_osfhandle((intptr_t)hout, _O_TEXT);
		FILE * fpout = _fdopen(hcout, "wt");
		*stdout = *fpout;
	}

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HIDEBYTOP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HIDEBYTOP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HIDEBYTOP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   {
	  auto style = ::GetWindowLong(hWnd, GWL_STYLE);
	  style &= (~(WS_CAPTION|WS_BORDER| WS_THICKFRAME));
	  ::SetWindowLong(hWnd, GWL_STYLE, style);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   {
	   RECT rect;
	   ::GetWindowRect(hWnd, &rect);
	   ::InvalidateRect(hWnd, &rect, true);
   }

   ::SetWindowPos(hWnd, HWND_TOPMOST, 0, GetSystemMetrics(SM_CYSCREEN) - 150, GetSystemMetrics(SM_CXSCREEN), 70, SWP_NOACTIVATE);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_ERASEBKGND:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		auto brush = ::CreateSolidBrush(RGB(0x0, 0x0, 0x0));
		auto hdc = ::GetWindowDC(hWnd);
		::FillRect(hdc, &rc, brush);
		DeleteObject(brush);
		::ReleaseDC(hWnd, hdc);
		return true;
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_LBUTTONDOWN:
		//第一种方法
		//#define  SC_DRAGMOVE  0xF012
		SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		//第二种方法
		//SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;
	case WM_KEYDOWN:
		std::cout << "WM_KEYDOWN\r\n";
		break;
	case WM_CHAR:
		std::cout << "WM_CHAR\r\n";
		break;
	case WM_KEYUP:
		std::cout << "WM_KEYUP\r\n";
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



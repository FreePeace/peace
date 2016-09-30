// HideByTop.cpp : Defines the entry point for the application.
//
#include "HideByTop.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Windowsx.h>
#include <iostream>  
#include <spdlog/spdlog.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

auto console = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	console->flush_on(spdlog::level::level_enum::info);
	
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
	  style &= (~(WS_CAPTION | WS_THICKFRAME));//|WS_THICKFRAME
	  style |= WS_CLIPCHILDREN | WS_TABSTOP | WS_OVERLAPPED;
	  ::SetWindowLong(hWnd, GWL_STYLE, style);

	  //style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	  //style &= (~(WS_CAPTION));//|WS_THICKFRAME
	  //style |= WS_CLIPCHILDREN | WS_TABSTOP | WS_OVERLAPPED;
	  ::SetWindowLong(hWnd, GWL_EXSTYLE, 0);
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
	case WM_SETCURSOR:
		switch (LOWORD(lParam))
		{
		case HTBOTTOM:
			SetCursor(LoadCursor(0, IDC_SIZENS));
			return 0;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_NCCALCSIZE:
	{ // Microsoft mentioned that if wParam is true, returning 0 should be enough, but after MoveWindow or similar functions it would begin to "wriggle"
		if (wParam)
		{
			int BORDERWIDTH = 4;
			NCCALCSIZE_PARAMS *Params = reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);
			Params->rgrc[0].bottom += BORDERWIDTH; // rgrc[0] is what makes this work, don't know what others (rgrc[1], rgrc[2]) do, but why not change them all?
			Params->rgrc[0].right += BORDERWIDTH;
			Params->rgrc[1].bottom += BORDERWIDTH;
			Params->rgrc[1].right += BORDERWIDTH;
			Params->rgrc[2].bottom += BORDERWIDTH;
			Params->rgrc[2].right += BORDERWIDTH;
			return 0;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	break;
	//case WM_SIZE:   //要让窗体能够随着缩放改变，要响应WM-SIZE消息  
	//{
	//	RECT rcClient = { 0 };
	//	::GetClientRect(hWnd, &rcClient);
	//	InvalidateRect(hWnd, &rcClient, FALSE);
	//}
	//break;
	case WM_NCHITTEST:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		//ScreenToClient(hWnd,&pt);
		//if (rc.PtInRect(pt) && rcWin.Width() > rcWin.Height())
		//	return HTMENU;

		if (true)
		{
			if (!::IsZoomed(hWnd))
			{
				const int margin = 4;
				RECT rc;
				GetWindowRect(hWnd,&rc);
				if (pt.x > rc.right - margin)
					return HTRIGHT;
				else if (pt.x < margin)
					return HTLEFT;
				else if (pt.y > rc.bottom - margin)
					return HTBOTTOM;
				else if (pt.y < margin)
					return HTTOP;
			}

			return HTCLIENT;
		}
		break;
	}
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
	{
		console->info("WM_KEYDOWN");
		RECT rc;
		::GetWindowRect(hWnd, &rc);
		int wmId = LOWORD(wParam);
		auto ctrl = (GetKeyState(VK_CONTROL) < 0);
		auto shift = (GetKeyState(VK_SHIFT) < 0);
		if (VK_LEFT == wmId)
		{
			if (ctrl)
			{
				rc.left--;
				if (rc.left < 0)
				{
					rc.left = 0;
				}
			}
			else if (shift)
			{
				rc.left++;
			}
			else
			{
				rc.left--;
				rc.right--;
			}
		}
		else if (VK_RIGHT == wmId)
		{
			if (ctrl)
			{
				rc.right++;
			}
			else if (shift)
			{
				rc.right--;
			}
			else
			{
				rc.left++;
				rc.right++;
			}
		}
		else if (VK_UP == wmId)
		{
			if (ctrl)
			{
				rc.top--;
			}
			else if (shift)
			{
				rc.top++;
			}
			else
			{
				rc.top--;
				rc.bottom--;
			}
		}
		else if (VK_DOWN == wmId)
		{
			if (ctrl)
			{
				rc.bottom++;
			}
			else if (shift)
			{
				rc.bottom--;
			}
			else
			{
				rc.top++;
				rc.bottom++;
			}
		}
		RECT cu;
		::GetWindowRect(hWnd, &cu);
		UINT flag = 0;
		{
			if (rc.right - rc.left == cu.right - cu.left && rc.bottom - rc.top == cu.bottom - cu.top)
			{
				flag = SWP_NOSIZE;
			}
			else
			{
				if (rc.right - rc.left < 10 && rc.bottom - rc.top < 10)
				{
					flag = SWP_NOSIZE;
				}
				//flag |= SWP_NOMOVE;
			}
		}
		::SetWindowPos(hWnd, HWND_TOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, flag);
		break;
	}
	case WM_CHAR:
		console->info("WM_CHAR\r\n");
		break;
	case WM_KEYUP:
		console->info("WM_KEYUP\r\n");
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



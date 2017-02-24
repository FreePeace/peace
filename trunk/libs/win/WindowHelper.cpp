#include "WindowHelper.h"
#include <WinUser.h>
namespace peace
{
	namespace win
	{
		WindowHelper::WindowHelper()
		{
		}

		WindowHelper::~WindowHelper()
		{
		}

		void WindowHelper::SetNoCaption(HWND wnd)
		{
			auto style = ::GetWindowLong(wnd, GWL_STYLE);
			style &= (~WS_CAPTION);
			::SetWindowLong(wnd, GWL_STYLE, style);
		}
		void WindowHelper::DragWindowNoCaption(HWND wnd)
		{
			//1
			::PostMessage(wnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			//2
			//::SendMessage(wnd,WM_SYSCOMMAND,SC_DRAGMOVE,0); 

			//拖动时无边框
			//SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, true, NULL, 0);
			//::SendMessage(wnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
	}
}

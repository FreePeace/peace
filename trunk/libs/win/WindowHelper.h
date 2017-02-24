#pragma once
#include <windef.h>
namespace peace
{
	namespace win
	{
		class WindowHelper
		{
		private:
			WindowHelper();
			~WindowHelper();

		public:
			static void SetNoCaption(HWND wnd);
			/// 在 WM_LBUTTONDOWN 消息中调用如下方法，
			/// 具体看原代码中的实现，有两种方式，以及无虚线框的实现
			static void DragWindowNoCaption(HWND wnd);

		private:

		};
	}
}
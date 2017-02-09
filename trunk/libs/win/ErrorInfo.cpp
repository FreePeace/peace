#include "ErrorInfo.h"
#include <windows.h>
#include <fstream>
namespace peace
{
	namespace win
	{
		ErrorInfo::ErrorInfo()
		{
		}

		ErrorInfo::~ErrorInfo()
		{
		}

		uint32_t ErrorInfo::LastErrorInfo(std::wstring & info)
		{
			DWORD eid = ::GetLastError();
			if (eid == 0)
			{
				info.clear();
			}
			else {
				LPWSTR messageBuffer = nullptr;
				size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, eid, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

				info = messageBuffer;
				LocalFree(messageBuffer);
			}
			return eid;
		}

		void ErrorInfo::AddTextToFile(const std::wstring & name, const std::wstring & text)
		{
			std::wofstream f;
			f.open(name, std::ios_base::app);
			if (f.is_open()) {
				f << text;
			}
		}

	}
}